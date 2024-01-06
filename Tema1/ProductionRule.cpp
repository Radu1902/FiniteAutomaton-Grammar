#include "ProductionRule.h"

const std::string& ProductionRule::GetLeftSide() const
{
    return m_leftSide;
}

const std::string& ProductionRule::GetRightSide() const
{
    return m_rightSide;
}

std::istream& operator>>(std::istream& in, ProductionRule& prod)
{
    in >> prod.m_leftSide >> prod.m_rightSide;
    return in;
}

std::ostream& operator<<(std::ostream& out, const ProductionRule& prod)
{
    out << prod.m_leftSide << " -> " << prod.m_rightSide;
    return out;
}
