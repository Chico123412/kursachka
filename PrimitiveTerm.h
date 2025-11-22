#ifndef KURSOVA_PRIMITIVETERM_H
#define KURSOVA_PRIMITIVETERM_H

#include "TermBase.h"

/**
 * Первинний термін – базове поняття без посилань на інші терміни.
 */
class PrimitiveTerm : public TermBase {
public:
    PrimitiveTerm();
    PrimitiveTerm(const std::string &name, const std::string &definition);

    PrimitiveTerm(const PrimitiveTerm &other);
    PrimitiveTerm(PrimitiveTerm &&other) noexcept;
    ~PrimitiveTerm() override;

    PrimitiveTerm &operator=(const PrimitiveTerm &other);
    PrimitiveTerm &operator=(PrimitiveTerm &&other) noexcept;

    std::string GetType() const override;
    bool IsPrimitive() const override;

    std::string Serialize() const override;
};

#endif //KURSOVA_PRIMITIVETERM_H
