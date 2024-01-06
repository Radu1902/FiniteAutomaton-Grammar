#pragma once
#include <string>
#include <iostream>

class ProductionRule
{
public:
	ProductionRule() = default;

	const std::string& GetLeftSide() const;
	const std::string& GetRightSide() const;

	friend std::istream& operator>>(std::istream& in, ProductionRule& prod);
	friend std::ostream& operator<<(std::ostream& out, const ProductionRule& prod);

private:
	std::string m_leftSide;
	std::string m_rightSide;
};

