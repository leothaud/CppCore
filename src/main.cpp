import core;
import ast;
import frontend;

core::Mutex mutex;
static constexpr int x_size = 1024 * 1024;
volatile int x[x_size];

__thread int local = 2;

struct X {};

void f(u64 n) {
  auto &logger = core::getThreadLogger();
  logger.info("Before: ", local);
  core::sleep(n);
  ++local;
  logger.info("After: ", local);
  core::stdLogger.info("thread ", n, " finished.");
}

int main() {
  int numThread = 8;
  auto **streams = new core::FileStream *[numThread];
  auto **loggers = new core::Logger *[numThread];
  for (int i = 0; i < numThread; ++i) {
    streams[i] = new core::FileStream("/tmp/stream_" + core::String::of(i));
    loggers[i] =
        new core::Logger(streams[i], streams[i], streams[i], true, true);
    core::startThread<f>(loggers[i], i);
  }
  return 0;
}