#include "cocaine/engine.hpp"
#include "cocaine/job.hpp"

using namespace cocaine;

class stdio_sink_t:
    public cocaine::logging::sink_t
{
    public:
        stdio_sink_t():
            cocaine::logging::sink_t(cocaine::logging::debug)
        { }

        virtual void emit(cocaine::logging::priorities, const std::string& message) const {
            std::cout << message << std::endl;
        }
};

class my_job_t:
    public cocaine::engine::job_t
{
    public:
        my_job_t(const std::string& event, const cocaine::blob_t& blob):
            cocaine::engine::job_t(event, blob)
        { }

    public:
        virtual void react(const cocaine::engine::events::chunk& event) {
            std::cout << "chunk: " << std::string((const char*)event.message.data(), event.message.size()) << std::endl;
        }
        
        virtual void react(const cocaine::engine::events::choke& event) {
            std::cout << "choke" << std::endl;
        }
        
        virtual void react(const cocaine::engine::events::error& event) {
            std::cout << "error: " << event.message << std::endl;
        }
};

int main() {
    const char data[] = "data";

    cocaine::config_t config;

    config.registry.modules = "modules/python";
    config.storage.driver = "files";
    config.storage.uri = "/var/lib/cocaine";
    config.runtime.self = "./cocained";
    config.sink.reset(new stdio_sink_t());

    cocaine::context_t context(config);

    cocaine::engine::threaded_engine_t engine(context, "test_app@1");

    engine.start();
    
    sleep(1);

    engine.enqueue(
        boost::make_shared<my_job_t>(
            "rimz_func",
            cocaine::blob_t(data, 5)
        )
    );

    sleep(1);

    Json::Value state(engine.info());
    
    std::cout << state;

    sleep(1000);

    return 0;
}
