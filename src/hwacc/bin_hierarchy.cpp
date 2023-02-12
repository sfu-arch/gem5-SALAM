#include "bin_hierarchy.hh"

namespace bin {
double CalcBandwidth(std::queue<SyncInfo> &queue) {
  if (queue.size() < 2) {
    return 0;
  }
  SyncInfo head = queue.front();
  SyncInfo tail = queue.back();
  double cycle_diff = tail.cycle - head.cycle;
  double size = head.size;
  return size / cycle_diff;
}

} // namespace bin