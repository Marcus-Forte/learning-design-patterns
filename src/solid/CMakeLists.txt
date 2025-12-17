add_executable(DependencyInversion dependency_inversion.cc)
add_executable(InterfaceSegregation interface_segregation.cc)
add_executable(LiskovSubstitution liskov_substitution.cc)
add_executable(OpenClose open_close.cc)
add_executable(SingleResponsability single_responsability.cc)

# Principle	Core Concept	C++ Implementation
# SRP	One job per class	Split big classes
# OCP	Extend, don't modify	virtual functions, override
# LSP	Subclasses must behave	Avoid breaking base class contracts
# ISP	No forced methods	Multiple inheritance of pure interfaces
# DIP	Decouple dependencies	Dependency Injection, Interfaces