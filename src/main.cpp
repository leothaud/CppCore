import core;

int fTest() {
  core::getThreadLogger()->info("Yo!");
  core::sleep(2);
  core::getThreadLogger()->info("End.");
  return 0;
}

int main() {
  core::sout << "Yo " << core::getColor<core::TEXT_COLORS::RED>("toi") << " !\n"
             << 0x123_x << "\n";
  return 0;
}