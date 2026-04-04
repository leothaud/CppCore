/* Copyright 2026 Dylan Leothaud
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
export module core:logger;

import :stream;
import :mutex;

namespace core {

//! Logger class used to redirrect outputs.
//! Has three output stream, for infos, for warnings and for errors.
//! For multithreaded execution, the logger can be configured as concurrent
//! or as shared to ensure non-entrelacing outputs.
export class Logger {
  FdStream *infoStream, *warningStream, *errorStream;
  bool labels;
  bool shared;
#ifdef CORE_THREAD
  Mutex lock;
#endif
  void infoImpl(auto elt) { *infoStream << elt; }
  void warningImpl(auto elt) { *warningStream << elt; }
  void errorImpl(auto elt) { *errorStream << elt; }

public:
  Logger(const Logger &other)
      : infoStream(other.infoStream), warningStream(other.warningStream),
        errorStream(other.errorStream), labels(other.labels),
        shared(other.shared)
#ifdef CORE_THREAD
        ,
        lock(other.lock)
#endif
  {
  }

  Logger &operator=(const Logger &other) {
    infoStream = other.infoStream;
    warningStream = other.warningStream;
    errorStream = other.errorStream;
    labels = other.labels;
    shared = other.shared;
#ifdef CORE_THREAD
    lock = other.lock;
#endif
    return *this;
  }
  Logger(Logger &&other)
      : infoStream(core::move(other.infoStream)),
        warningStream(core::move(other.warningStream)),
        errorStream(core::move(other.errorStream)),
        labels(core::move(other.labels)), shared(core::move(other.shared))
#ifdef CORE_THREAD
        ,
        lock(core::move(other.lock))
#endif
  {
  }

  Logger &operator=(Logger &&other) {
    infoStream = core::move(other.infoStream);
    warningStream = core::move(other.warningStream);
    errorStream = core::move(other.errorStream);
    labels = core::move(other.labels);
    shared = core::move(other.shared);
#ifdef CORE_THREAD
    lock = core::move(other.lock);
#endif
    return *this;
  }

  constexpr Logger(FdStream *infoStream, FdStream *warningStream,
                   FdStream *errorStream, bool shared = true,
                   bool labels = true)
      : infoStream(infoStream), warningStream(warningStream),
        errorStream(errorStream), labels(labels), shared(shared) {}
  constexpr Logger() : Logger(nullptr, nullptr, nullptr) {}

  template <typename... T> void info(T... elts) {
#ifdef CORE_THREAD
    if (shared) {
      lock.acquire();
    }
#endif
    if (labels) {
      infoImpl("[Info]: ");
    }
    (infoImpl(forward<T>(elts)), ...);
    infoImpl("\n");
#ifdef CORE_THREAD
    if (shared) {
      lock.release();
    }
#endif
  }

  template <typename... T> void warning(T... elts) {
#ifdef CORE_THREAD
    if (shared) {
      lock.acquire();
    }
#endif
    if (labels) {
      warningImpl("[Warning]: ");
    }
    (warningImpl(forward<T>(elts)), ...);
    warningImpl("\n");
#ifdef CORE_THREAD
    if (shared) {
      lock.release();
    }
#endif
  }

  template <typename... T> void error(T... elts) {
#ifdef CORE_THREAD
    if (shared) {
      lock.acquire();
    }
#endif
    if (labels) {
      errorImpl("[Error]: ");
    }
    (errorImpl(forward<T>(elts)), ...);
    errorImpl("\n");
#ifdef CORE_THREAD
    if (shared) {
      lock.release();
    }
#endif
  }

  bool isShared() const { return shared; }
  void setShared() { shared = true; }
  void setShared(bool shared) { this->shared = shared; }
  void unsetShared() { shared = false; }
};

//! Standard output logger. infos/warnings -> stdout. errors -> stderr
export Logger stdLogger;

//! Get a thread-local logger. Initial value is stdLogger, but can be changed.
export [[gnu::always_inline]] Logger *&getThreadLogger() {
#ifdef CORE_THREAD
  static thread_local Logger *logger = &stdLogger;
#else
  static Logger *logger = &stdLogger;
#endif
  return logger;
}
//! Reset the thread logger value to stdLogger
export void resetThreadLogger() { getThreadLogger() = &stdLogger; }

//! Initialize stdLooger, necessary because of global variable initialisation
//! order.
export void initializeLoggers() { stdLogger = Logger(&sout, &sout, &serr); }

} // namespace core