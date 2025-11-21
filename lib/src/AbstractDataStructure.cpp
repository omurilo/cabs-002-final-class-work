#include "AbstractDataStructure.hpp"

namespace ds {

void AbstractDataStructure::notify() {
    // Biblioteca core não dispara nada por padrão.
    // A aplicação pode derivar ou fornecer wrapper que chama callbacks.
}

} // namespace ds
