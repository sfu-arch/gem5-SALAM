#ifndef __BIN_HIERARCHY__
#define __BIN_HIERARCHY__

#include "params/CommInterface.hh"
#include "map"

template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}

struct Bundle {
  int id;
  int size;
  std::pair<int, int> position;
  uint64_t last_touched_cycle;

  Bundle(int id, int size) {
    this->id = id;
    this->size = size;
  }
};


class GenericBin {
  public:
    GenericBin(uint32_t size):  capacity(size) {}
    GenericBin(): capacity(10) {}

    void push(uint32_t count) {
      if (capacity < count) {
        spillToMemory(count);
      }
      else
        capacity-= count;

    }
    std::vector<Addr> pop(uint32_t count) {
      auto fetch_amount = std::min((uint64_t)count, in_mem_count);
      capacity+= (count - fetch_amount);
      if (fetch_amount)
        return fetchFromMemory(fetch_amount);
      return std::vector<Addr>();
    }
    double read() {
      return 1;
    }
    void write() {}

    void spillToMemory(uint32_t count) {
      // spill to memory
      for (int i = 0; i < count; i++) {
        // Addr memAddr = (current_address)+i*data_size;
        current_address += data_size;
      }
      std::cerr << "Total wrote " << std::dec << spilled_count * data_size << std::endl;

      in_mem_count += count;
      spilled_count += count;
    }
    std::vector<Addr> fetchFromMemory(uint32_t count) {
      // fetch from memory
      std::vector<Addr> addresses;
      for (int i = 0; i < count; i++) {
        Addr memAddr = (current_address);
        addresses.push_back(memAddr);
      }
      std::cerr << "Total Fetched " << std::dec << fetched_count * data_size << std::endl;
      in_mem_count -= count;
      fetched_count += count;
      return addresses;
    }
  private:
    uint64_t capacity;
    uint64_t base_address = (uint64_t)0x80c00000;
    uint64_t current_address = base_address;

    uint64_t in_mem_count = 0;
    uint64_t spilled_count = 0;
    uint64_t fetched_count = 0;

    const uint16_t data_size = sizeof(double);

};


class BinHierarchy {
  public:
    BinHierarchy(uint32_t count, std::vector<int> sizes):  bin_count(count), bin_sizes(sizes) {
      std::cerr << "Bin Count = " << bin_count << std::endl;
      auto first_bin = new ZeroBin(0, sizes[0], this);
      bins.push_back(first_bin);
      for (int i = 1; i < count; i++) {
        std::cerr << "size[" << i << "] = " << sizes[i] << std::endl;
        bins.push_back(new IBin(i, sizes[i], this));
        bins[i-1]->next_bin = bins[i];
      }
      last_bin = new IBin(count, 1000000, this);
    }

  class IBin {
    public:
      IBin(uint32_t id, uint32_t size, BinHierarchy* h): id(id), capacity(size), hierarchy(h) {}
      IBin(uint32_t id, BinHierarchy* h): IBin(id, 10, h) {}
      void push(Bundle *bundle) {
        if (capacity < bundle->size) {
          handleSpill(bundle);       
        } else {
          std::cerr << id << " : Didn't spill" << std::endl;;
        }
        current_bundles.push_back(bundle);
        capacity-= bundle->size;
      
      }

      Bundle* pop() {
        Bundle* to_return;

        if (current_bundles.size() == 0) {
          if (next_bin)
            return next_bin->pop();
          else
            return hierarchy->fetchFromMemory();
        } else {
          to_return = current_bundles.back();
          capacity+= to_return->size;
          current_bundles.pop_back();
          return to_return;
        }
        // TODO: handle fill requests from the next bin
      }

      void handleSpill(Bundle *bundle) {
        while (capacity < bundle->size) {
          if (next_bin) {   // spill to next bin
            std::cerr << "Spilling from " << id  << " to " << id + 1 << ", count: " << current_bundles.front()->size << std::endl;
            next_bin->push(current_bundles.front());
          } else {  // spill to memory  
            std::cerr << id << " : Spilling to mem " << std::endl;;

            hierarchy->spillToMemory(current_bundles.front());
          }
          capacity += current_bundles.front()->size;
          pop_front<Bundle*>(current_bundles);
        }
      }
      uint32_t capacity;
      uint32_t id;
      uint64_t base_address = (uint64_t)0x80c00000;

      BinHierarchy* hierarchy;
      IBin *next_bin = nullptr;
      std::vector<Bundle*> current_bundles;
  };
  
  class ZeroBin: public IBin {
    public:
      ZeroBin(uint32_t id, uint32_t size, BinHierarchy* h): IBin(id, size, h) {}
      ZeroBin(uint32_t id, BinHierarchy* h): IBin(id, 10, h) {}
      double read() {
        return 0;
      }

      void write() {}

  };
  
  void push(uint32_t count) {
    Bundle *bundle = new Bundle(last_bundle_id++, count);
    bundle_map[bundle->id] = bundle;
    bins[0]->push(bundle);
  }

  void pop() {
    if (!bundle_map.size())
      return;

    auto bundle = bins[0]->pop();
    bundle_map.erase(bundle->id);
  }

  void spillToMemory(Bundle *bundle) {
    // spill to memory
    current_address += data_size * bundle->size;
    in_mem_count += bundle->size;
    spilled_count += bundle->size;
    last_bin->push(bundle);
    std::cerr << "Total wrote " << std::dec << spilled_count << std::endl;
  }
  
  Bundle* fetchFromMemory() {
    // fetch from memory
    
    auto to_ret = last_bin->pop();
    in_mem_count -= to_ret->size;
    fetched_count += to_ret->size;
    std::cerr << "Total Fetched " << std::dec << fetched_count << std::endl;
    return to_ret;
  }

  std::vector<IBin*> bins;
  std::map<int, Bundle*> bundle_map;
  IBin* last_bin;

  int last_bundle_id = 0;

  const uint16_t data_size = sizeof(double);
  uint64_t base_address = (uint64_t)0x80c00000;
  uint64_t current_address = base_address;
  
  uint64_t in_mem_count = 0;
  uint64_t spilled_count = 0;
  uint64_t fetched_count = 0;
  
  uint64_t bin_count;
  std::vector<int> bin_sizes;
};
#endif //__BIN_HIERARCHY__
