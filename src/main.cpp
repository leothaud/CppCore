/* Copyright 2026 Dylan Leothaud
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *
 *
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
import core;
import ast;
import frontend;

int main(int argc, char **argv) {
  if (argc != 2) {
    core::serr << "Usage: " << argv[0] << " [test file].\n";
    return 1;
  }
  auto result = compiler::frontend::parseFile(argv[1]);
  if (result.failed()) {
    core::serr << result.getMessage() << "\n";
    return 1;
  }
  core::StringStream stream;
  auto prog = core::move(result.getValue().first);
  prog->prettyPrint(stream);
  core::String prettyPrinted = stream.toString();
  auto result2 = compiler::frontend::parse(prettyPrinted);
  if (result2.failed()) {
    core::serr << result2.getMessage() << "\n";
    return 1;
  }
  auto prog2 = core::move(result2.getValue().first);
  auto comparison = prog->isEqual(*prog2);
  if (comparison.failed()) {
    core::serr << comparison.getMessage() << "\n-------------\n";
    prog->prettyPrint(core::serr) << "\n-------------\n";
    prog2->prettyPrint(core::serr) << "\n";
    return 1;
  }
  core::sout << "Ok\n";
  return 0;
}