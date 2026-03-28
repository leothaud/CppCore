import core;

struct Base {
  virtual void doPrint() {};
};

struct A : public Base {
  void doPrint() { core::getThreadLogger()->info("A"); }
};

struct B : public Base {
  virtual void doPrint() override { core::getThreadLogger()->info("B"); }
};

void go(Base *base) {
  // if (auto *a = dynamic_cast<A *>(base)) {
  //   a->doPrint();
  // } else {
  base->doPrint();
  // }
}

int main() {
  A a;
  go(&a);
  // core::sout << "Yo " << core::getColor<core::TEXT_COLORS::RED>("toi") << "
  // !\n"
  //            << core::getColor<core::TEXT_COLORS::BLUE>(0x123_x) << " - " <<
  //            12
  //            << "\n";
  return 0;
}