#include <Memory.hpp>

void memory(const Memory& mem, const uintptr_t clientDll) {
    const auto entityList = mem.read<uintptr_t>(clientDll + 0x01873F18);
    std::cout << "entityList: " << std::hex << entityList << std::dec << "\n";

    const auto entity = mem.read<uintptr_t>(entityList + 0x10);
    std::cout << "entity: " << std::hex << entity << std::dec << "\n";
}

int main() {
    const Memory mem(L"cs2.exe");
    auto clientDll = mem.getModule(L"client.dll");
    std::cout << "client.dll: " << std::hex << clientDll << std::dec << "\n";

    memory(mem, clientDll);
    return 0;
}
