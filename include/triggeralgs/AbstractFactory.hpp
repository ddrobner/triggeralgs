/* @file: AbstractFactory.hpp
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2023.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_ABSTRACT_FACTORY_HPP_
#define TRIGGERALGS_ABSTRACT_FACTORY_HPP_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace triggeralgs {

template <typename T>
class AbstractFactory
{
  using makerCreator = std::function<std::unique_ptr<T>()>;
  using creationMap = std::unordered_map<std::string, makerCreator>;

  public:
    AbstractFactory(const AbstractFactory&) = delete;
    AbstractFactory& operator=(const AbstractFactory&) = delete;
    virtual ~AbstractFactory() {}

    static std::unique_ptr<T> buildMaker(const std::string& algName);

    static void registerCreator(const std::string algName, makerCreator creator);

  private:
    static creationMap& getMakers();
};

} /* namespace triggeralgs */

#include "triggeralgs/AbstractFactory.hxx"

// TODO: Define ers exceptions
#endif // TRIGGERALGS_ABSTRACT_FACTORY_HPP_
