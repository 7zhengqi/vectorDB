#include "common/vector_cfg.h"
#include "httpserver/http_server.h"
#include "index/index_factory.h"
#include "logger/logger.h"
#include "common/vector_init.h"


// NOLINTNEXTLINE
auto main() -> int {
    vectordb::Init();
    vectordb::global_logger->info("Global IndexFactory initialized");

    // 创建并启动HTTP服务器
    std::string host = "localhost";
    vectordb::VectorDatabase vector_database(vectordb::Cfg::Instance().RocksDbPath(),vectordb::Cfg::Instance().WalPath());
    vector_database.ReloadDatabase();
    vectordb::HttpServer server(host, vectordb::Cfg::Instance().Port(),&vector_database);
    vectordb::global_logger->info("HttpServer created");
    server.Start();

    return 0;
}
