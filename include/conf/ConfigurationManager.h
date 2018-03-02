#ifndef CONF_CONFIGURATIONMANAGER_H
#define CONF_CONFIGURATIONMANAGER_H

#include "conf/Export.h"
#include "conf/Configuration.h"
#include "conf/Singleton.h"

class QDomElement;

namespace dml
{

namespace conf
{

/** Implementation of the ConfigurationManager class. */
class CONF_EXPORT ConfigurationManager : public Singleton<ConfigurationManager>
{
public:
    ConfigurationManager() {}

    void loadConfigurationData(const QDomElement &config);
    Configuration configuration() const { return _configuration; }

private:
    void loadFramesConfiguration(const QDomElement& frames);
    void loadAudioConfiguration(const QDomElement& audio);

    QStringList loadPatternList(const QDomElement &root, const QString &childName) const;
    QString readDomElementValue(const QDomElement &root, const QString &elementName) const;

    Configuration _configuration;

}; // class ConfigurationManager

} // namespace conf

} // namespace dml

#endif // CONF_CONFIGURATIONMANAGER_H