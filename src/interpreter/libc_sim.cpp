#include "libc_sim.h"

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>

#include "container_utils.h"

// IO
namespace ravel::libc {

void puts(std::array<std::int32_t, 32> &regs, std::vector<std::byte> &storage,
          FILE *fp) {
  std::size_t pos = regs[10];
  regs[10] = std::fputs((const char *)(storage.data() + pos), fp);
}

void scanf(std::array<std::int32_t, 32> &regs, std::vector<std::byte> &storage,
           FILE *fp) {
  auto fmtStr = (const char *)(storage.data() + regs[10]);
  std::size_t assigned = 0;
  bool succeeded = true;
  for (auto iter = fmtStr; *iter != '\0' && succeeded; ++iter) {
    auto fmtCh = *iter;
    if (fmtCh == '\\') {
      ++iter;
      fmtCh = *iter;
      assert(fmtCh == 'n');
      fmtCh = '\n';
    }

    if (isspace(fmtCh)) {
      int ch = std::fgetc(fp);
      while (std::isspace(ch))
        ch = std::getc(fp);
      std::ungetc(ch, fp);
      continue;
    }

    if (fmtCh == '%') {
      ++iter;
      fmtCh = *iter;
      assert(fmtCh == 'd');
      succeeded =
          std::fscanf(fp, "%d", (int *)(storage.data() + regs[11 + assigned]));
      assigned += succeeded;
      continue;
    }

    char ch = std::fgetc(fp);
    while (std::isspace(ch))
      ch = std::fgetc(fp);
    succeeded = ch == fmtCh;
  }
  regs[10] = assigned;
}

void printf(std::array<std::int32_t, 32> &regs,
            const std::vector<std::byte> &storage, FILE *fp) {
  auto fmtStr = std::string((const char *)(storage.data() + regs[10]));
  std::size_t nSuccess = 0;
  std::size_t curArg = 0;
  for (std::size_t i = 0; i < fmtStr.size(); ++i) {
    if (fmtStr[i] == '\\') {
      ++i;
      assert(i != fmtStr.size());
      assert(fmtStr[i] == 'n'); // TODO
      std::putc('\n', fp);
      continue;
    }
    if (fmtStr[i] == '%') {
      ++i;
      assert(i != fmtStr.size());
      assert(fmtStr[i] == 'd'); // TODO
      std::fprintf(fp, "%d", regs[11 + curArg]);
      ++curArg;
      ++nSuccess;
      continue;
    }
    std::putc(fmtStr[i], fp);
  }
  regs[10] = nSuccess;
}

void putchar(std::array<std::int32_t, 32> &regs, FILE *fp) {
  regs[10] = std::putc(regs[10], fp);
}

} // namespace ravel::libc

namespace ravel {

void libc::malloc(std::array<std::int32_t, 32> &regs,
                  const std::vector<std::byte> &storage, std::size_t &heapPtr,
                  std::unordered_set<std::size_t> &malloced) {
  auto size = (std::size_t)regs[10];
  regs[10] = heapPtr;
  malloced.emplace(heapPtr);
  heapPtr += size;
  assert(heapPtr < storage.size());
}

void libc::free(const std::array<std::int32_t, 32> &regs,
                std::unordered_set<std::size_t> &malloced) {
  std::size_t addr = regs[10];
  assert(isIn(malloced, addr));
  malloced.erase(malloced.find(addr));
}

void libc::memcpy(std::array<std::int32_t, 32> &regs,
                  std::vector<std::byte> &storage) {
  std::size_t dest = regs[10];
  std::size_t src = regs[11];
  std::size_t cnt = regs[12];
  assert(src + cnt < storage.size() && dest + cnt < storage.size());
  std::memcpy(storage.data() + dest, storage.data() + src, cnt);
}

void libc::strlen(std::array<std::int32_t, 32> &regs,
                  const std::vector<std::byte> &storage) {
  std::size_t strPos = regs[10];
  regs[10] = std::strlen((char *)storage.data() + strPos);
}

void libc::strcpy(std::array<std::int32_t, 32> &regs,
                  std::vector<std::byte> &storage) {
  std::size_t dest = regs[10], src = regs[11];
  std::strcpy((char *)storage.data() + dest, (char *)storage.data() + src);
}

void libc::memset(std::array<std::int32_t, 32> &regs,
                  std::vector<std::byte> &storage) {
  std::size_t dest = regs[10];
  int ch = regs[11];
  std::size_t cnt = regs[12];
  std::memset(storage.data() + dest, ch, cnt);
}

} // namespace ravel