#ifndef __DML_CORE_INFORMATION_H__
#define __DML_CORE_INFORMATION_H__

#include "Export.h"

namespace dml {
namespace core {

class CORE_EXPORT Information {
public:
    Information()
        : _hasPublicationDate(false)
    {}

    // We need to reimplement comparison operators because of 'hasPublication'
    bool isEqual(const Information& rhs) const
    {
        return _producer==rhs._producer &&
               _copyright==rhs._copyright &&
               _description==rhs._description &&
               _hasPublicationDate==rhs._hasPublicationDate &&
               (!_hasPublicationDate || (_publicationDate==rhs._publicationDate));
    }

    bool operator==(const Information& rhs) const { return isEqual(rhs); }
    bool operator!=(const Information& rhs) const { return !isEqual(rhs); }

    void setProducer(const std::string &producer);
    std::string producer() const { return _producer; }

    void setCopyright(const std::string &copyright);
    std::string copyright() const { return _copyright; }

    void setDescription(const std::string &description);
    std::string description() const { return _description; }

    bool hasPublicationDate() const { return _hasPublicationDate; }
    void setPublicationDate(const std::string &publicationDate);
    std::string publicationDate() const { return _hasPublicationDate ? _publicationDate : std::string(); }

    bool isNull() const;
    void clear();

private:
    std::string _producer;
    std::string _copyright;
    std::string _description;
    bool _hasPublicationDate;
    std::string _publicationDate;
}; // class Information

} // namespace core
} // namespace dml

#endif // __DML_CORE_INFORMATION_H__
