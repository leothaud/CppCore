import core;
import ast;
import frontend;

core::Mutex mutex;

struct X {
  int x;
  X(int x) : x(x) {}
  ~X() {}
  X &operator++() {
    ++x;
    return *this;
  }
  operator int() { return x; }
};

thread_local X local = 2;

void f(u64 n) {
  auto &logger = core::getThreadLogger();
  logger.info("Before: ", local);
  core::sleep(n);
  ++local;
  logger.info("After: ", local);
  core::stdLogger.info("thread ", n, " finished.");
}

int main() {
  int numThread = 48;
  for (int i = 0; i < numThread; ++i) {
    core::startThread<f>(i);
  }
  return 0;
}