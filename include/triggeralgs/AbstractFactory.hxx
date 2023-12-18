/* @file: AbstractFactory.hxx
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2023.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef TRIGGERALGS_ABSTRACT_FACTORY_HXX_
#define TRIGGERALGS_ABSTRACT_FACTORY_HXX_

namespace triggeralgs {

template <typename T>
typename AbstractFactory<T>::creationMap& AbstractFactory<T>::getMakers(){
  static creationMap s_makers;
  return s_makers;
}

template <typename T>
void AbstractFactory<T>::registerCreator(const std::string algName, makerCreator creator)
{
  creationMap& makers = getMakers();
  auto it = makers.find(algName);

  if (it == makers.end()) {
    makers[algName] = creator;
    return;
  }
  return;
}

template <typename T>
std::shared_ptr<T> AbstractFactory<T>::buildMaker(const std::string& algName)
{
  creationMap& makers = getMakers();
  auto it = makers.find(algName);

  if (it != makers.end()) {
    return it->second();
  }

  return nullptr;
}

} // namespace triggeralgs

#endif // TRIGGERALGS_ABSTRACT_FACTORY_HXX_
