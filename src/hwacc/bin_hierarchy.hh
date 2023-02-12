#ifndef __BIN_HIERARCHY__
#define __BIN_HIERARCHY__

#include "map"
#include "params/CommInterface.hh"
#include <deque>
#include <queue>

namespace bin {
namespace {
template <typename T> void pop_front(std::vector<T> &vec) {
  assert(!vec.empty());
  vec.erase(vec.begin());
}

struct Bundle {
  int id;
  int size;
  std::pair<int, int> position;
  uint64_t last_touched_cycle;

  Bundle(int id, int size) : id(id), size(size) {}
};
} // namespace

// Contains the information about a push/pop to measure the memory bandwidth.
struct SyncInfo {
  uint64_t cycle;
  uint64_t size;

  SyncInfo(uint64_t cycle, uint64_t size) : cycle(cycle), size(size) {}
};

// A fixed size queue to store the SyncInfo and used for calculating the
// bandwidth. Example usage:
// FixedQueue<SyncInfo, 2> queue;
// queue.push(SyncInfo(1, 10));
// queue.push(SyncInfo(2, 20));
// SyncInfo head = queue.front(); // returns SyncInfo(1, 10)
// SyncInfo tail = queue.back(); // returns SyncInfo(2, 20)
// cycle_diff = tail.cycle - head.cycle;
// size_diff = tail.size - head.size;
// bandwidth = size_diff / cycle_diff;
template <typename T, int MaxLen, typename Container = std::deque<T>>
class FixedQueue : public std::queue<T, Container> {
public:
  void push(const T &value) {
    if (this->size() == MaxLen) {
      this->c.pop_front();
    }
    std::queue<T, Container>::push(value);
  }
};

// Computes the bandwidth from the front and back of the queue.
double CalcBandwidth(std::queue<SyncInfo> &queue);

class BinHierarchy {
public:
  BinHierarchy(uint32_t count, std::vector<int> sizes)
      : bin_count(count), bin_sizes(sizes) {
    std::cerr << "Bin Count = " << bin_count << std::endl;
    auto first_bin = new ZeroBin(0, sizes[0], this);
    bins.push_back(first_bin);
    for (int i = 1; i < count; i++) {
      std::cerr << "size[" << i << "] = " << sizes[i] << std::endl;
      bins.push_back(new IBin(i, sizes[i], this));
      bins[i - 1]->next_bin = bins[i];
    }
    last_bin = new IBin(count, 100000000, this);
  }

  class IBin {
  public:
    IBin(uint32_t id, uint32_t size, BinHierarchy *h)
        : id(id), capacity(size), hierarchy(h) {}
    IBin(uint32_t id, BinHierarchy *h) : IBin(id, 10, h) {}
    void push(Bundle *bundle) {
      if (capacity < bundle->size) {
        handleSpill(bundle->size);
      } else {
        std::cerr << id << " : Didn't spill" << std::endl;
        ;
      }
      current_bundles.push_back(bundle);
      capacity -= bundle->size;
    }

    void push(int size) {
      if (capacity < size) {
        handleSpill(size);
      } else {
        std::cerr << id << " : Didn't spill" << std::endl;
        ;
      }
      capacity -= size;
    }

    Bundle *pop() {
      Bundle *to_return;
      return nullptr;
      if (current_bundles.size() == 0) {
        if (next_bin)
          return next_bin->pop();
        else
          return hierarchy->fetchFromMemory();
      } else {
        to_return = current_bundles.back();
        capacity += to_return->size;
        current_bundles.pop_back();
        return to_return;
      }
      // TODO: handle fill requests from the next bin
    }

    void handleSpill(int size) {
      while (capacity < size && current_bundles.size() > 0) {
        if (next_bin) { // spill to next bin
          std::cerr << "Spilling from " << id << " to " << id + 1
                    << ", count: " << current_bundles.front()->size
                    << std::endl;
          next_bin->push(current_bundles.front());
        } else { // spill to memory
          std::cerr << id << " : Spilling to mem " << std::endl;
          ;

          hierarchy->spillToMemory(current_bundles.front());
        }
        capacity += current_bundles.front()->size;
        pop_front<Bundle *>(current_bundles);
      }
    }
    uint32_t capacity;
    uint32_t id;
    uint64_t base_address = (uint64_t)0x80c00000;

    BinHierarchy *hierarchy;
    IBin *next_bin = nullptr;
    std::vector<Bundle *> current_bundles;
  };

  class ZeroBin : public IBin {
  public:
    ZeroBin(uint32_t id, uint32_t size, BinHierarchy *h) : IBin(id, size, h) {}
    ZeroBin(uint32_t id, BinHierarchy *h) : IBin(id, 10, h) {}
    double read() { return 0; }

    void write() {}
  };

  void push(uint32_t count) {
    // Bundle *bundle = new Bundle(last_bundle_id++, count);
    // bundle_map[bundle->id] = bundle;
    // bins[0]->push(bundle);
    // std::cerr << "Push Done " << std::endl;
    spillToMemory(count);
  }
  void pop(int size) { fetched_count += size; }
  void pop() {
    return;
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

  void spillToMemory(int size) {
    // spill to memory
    current_address += data_size * size;
    in_mem_count += size;
    spilled_count += size;
    std::cerr << "Total wrote " << std::dec << spilled_count << std::endl;
  }

  Bundle *fetchFromMemory() {
    // fetch from memory

    auto to_ret = last_bin->pop();
    in_mem_count -= to_ret->size;
    fetched_count += to_ret->size;
    std::cerr << "Total Fetched " << std::dec << fetched_count << std::endl;
    return to_ret;
  }

  std::vector<IBin *> bins;
  std::map<int, Bundle *> bundle_map;
  IBin *last_bin;

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
} // namespace bin

#endif //__BIN_HIERARCHY__
