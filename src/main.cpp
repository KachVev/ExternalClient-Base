#include <ios>
#include <iostream>

#include "include/memory.hpp"
#include "include/signature.hpp"
#include "math/vector2.hpp"
#include "math/vector3.hpp"

void memory(const Memory& mem, const uintptr_t clientDll) {
    const auto entityList = mem.read<uintptr_t>(clientDll + 0x01873F18);
    std::cout << "entityList: " << std::hex << entityList << std::dec << "\n";

    const auto entity = mem.read<uintptr_t>(entityList + 0x10);
    std::cout << "entity: " << std::hex << entity << std::dec << "\n";
}

void signature(const Memory& mem, const uintptr_t clientDll) {
    const Signature sig("48 8B ?? ?? ?? ?? ?? 48 8B ?? ?? ??");
    const uintptr_t found = sig.find(mem, clientDll);

    std::cout << "Found at: 0x" << std::hex << found << std::dec << "\n";
}

void move(const Vector2& v) {
    std::cout << "Moving to: ";
    v.print();
}

void vector() {
    const Vector2 v2(3, 4);
    const Vector3 v3(4, 3, 12);

    move(v2);
    move(v3);

    v2.print();
    v3.print();
}


int main() {
    const Memory mem(L"cs2.exe");
    auto clientDll = mem.getModule(L"client.dll");
    std::cout << "client.dll: " << std::hex << clientDll << std::dec << "\n";

    memory(mem, clientDll);
    signature(mem, clientDll);
    vector();
    return 0;
}

