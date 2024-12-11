#include "index/faiss_index.h"
#include <faiss/IndexIDMap.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include "common/constants.h"
#include "logger/logger.h"

namespace vectordb {
FaissIndex::FaissIndex(faiss::Index *index) : index_(index) {}

void FaissIndex::InsertVectors(const std::vector<float> &data, int64_t label) {
  auto id = static_cast<int64_t>(label);
  index_->add_with_ids(1, data.data(), &id);
}

auto FaissIndex::SearchVectors(const std::vector<float> &query, int k)
    -> std::pair<std::vector<int64_t>, std::vector<float>> {
  int dim = index_->d;
  int num_queries = query.size() / dim;
  std::vector<int64_t> indices(num_queries * k);
  std::vector<float> distances(num_queries * k);

  index_->search(num_queries, query.data(), k, distances.data(), indices.data());

  global_logger->debug("Retrieved values:");
  for (size_t i = 0; i < indices.size(); ++i) {
    if (indices[i] != -1) {
      global_logger->debug("ID: {}, Distance: {}", indices[i], distances[i]);
    } else {
      global_logger->debug("No specific value found");
    }
  }
  return {indices, distances};
}

void FaissIndex::RemoveVectors(const std::vector<int64_t>& ids) { // 添加remove_vectors函数实现
    auto* id_map = dynamic_cast<faiss::IndexIDMap*>(index_);
    if (index_ != nullptr) {
        // 初始化IDSelectorBatch对象
        faiss::IDSelectorBatch selector(ids.size(), ids.data());
        auto remove_size = id_map->remove_ids(selector);
        global_logger->debug("remove size = {}",remove_size);
    } else {
        throw std::runtime_error("Underlying Faiss index is not an IndexIDMap");
    }
}

}  // namespace vectordb