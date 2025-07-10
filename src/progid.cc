#include "progid.h"

/*!
 * \brief ProgId::ProgId
 */
ProgId::ProgId() {}

/*!
 * \brief ProgId::strInternalName
 * \return
 */
const QString
ProgId::strInternalName()
{
  return QString(Name);
}

/*!
 * \brief ProgId::strName
 * \return QString() name program
 */
const QString
ProgId::strName()
{
  return QString(Name);
}

const QString
ProgId::strNickName()
{
  return QString(NickName);
}

/*!
 * \brief ProgId::strVersion
 * \return QString version program
 */
const QString
ProgId::strVersion()
{
  return QString(Version);
}

/*!
 * \attention The name VCPLinux below should not be changed without due care.
 * \brief ProgId::strOrganization
 *
 * \return name of organization
 */
const QString
ProgId::strOrganization()
{
  return QString("VCPLinux");
}
