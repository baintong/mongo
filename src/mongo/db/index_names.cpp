/**
*    Copyright (C) 2013 10gen Inc.
*
*    This program is free software: you can redistribute it and/or  modify
*    it under the terms of the GNU Affero General Public License, version 3,
*    as published by the Free Software Foundation.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*    As a special exception, the copyright holders give permission to link the
*    code of portions of this program with the OpenSSL library under certain
*    conditions as described in each individual source file and distribute
*    linked combinations including the program with the OpenSSL library. You
*    must comply with the GNU Affero General Public License in all respects for
*    all of the code used other than as permitted herein. If you modify file(s)
*    with this exception, you may extend this exception to your version of the
*    file(s), but you are not obligated to do so. If you do not wish to do so,
*    delete this exception statement from your version. If you delete this
*    exception statement from all source files in the program, then also delete
*    it in the license file.
*/

#include "mongo/db/index_names.h"

#include "mongo/db/commands/test_commands_enabled.h"
#include "mongo/db/jsobj.h"

namespace mongo {

using std::string;

const string IndexNames::GEO_2D = "2d";
const string IndexNames::GEO_HAYSTACK = "geoHaystack";
const string IndexNames::GEO_2DSPHERE = "2dsphere";
const string IndexNames::TEXT = "text";
const string IndexNames::HASHED = "hashed";
const string IndexNames::BTREE = "";
const string IndexNames::ALLPATHS = "allPaths";

const StringMap<IndexType> kIndexNameToType = {
    {IndexNames::GEO_2D, INDEX_2D},
    {IndexNames::GEO_HAYSTACK, INDEX_HAYSTACK},
    {IndexNames::GEO_2DSPHERE, INDEX_2DSPHERE},
    {IndexNames::TEXT, INDEX_TEXT},
    {IndexNames::HASHED, INDEX_HASHED},
    {IndexNames::ALLPATHS, INDEX_ALLPATHS},
};

// static
string IndexNames::findPluginName(const BSONObj& keyPattern) {
    BSONObjIterator i(keyPattern);
    while (i.more()) {
        BSONElement e = i.next();
        StringData fieldName(e.fieldNameStringData());
        if (String == e.type()) {
            return e.String();
        } else if ((fieldName == "$**") || fieldName.endsWith(".$**")) {
            return IndexNames::ALLPATHS;
        } else
            continue;
    }

    return IndexNames::BTREE;
}

// static
bool IndexNames::existedBefore24(const string& name) {
    return name == IndexNames::BTREE || name == IndexNames::GEO_2D ||
        name == IndexNames::GEO_HAYSTACK || name == IndexNames::HASHED;
}

// static
bool IndexNames::isKnownName(const string& name) {
    return name == IndexNames::GEO_2D || name == IndexNames::GEO_2DSPHERE ||
        name == IndexNames::GEO_HAYSTACK || name == IndexNames::TEXT ||
        name == IndexNames::HASHED || name == IndexNames::BTREE ||
        (getTestCommandsEnabled() && name == IndexNames::ALLPATHS);
}

// static
IndexType IndexNames::nameToType(StringData accessMethod) {
    auto typeIt = kIndexNameToType.find(accessMethod);
    if (typeIt == kIndexNameToType.end()) {
        return INDEX_BTREE;
    }
    return typeIt->second;
}

}  // namespace mongo
