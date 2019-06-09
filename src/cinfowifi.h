#ifndef _CINFOWIFI_H_
#define _CINFOWIFI_H_

#include <iostream> // ostream

#include "ccoordinate.h"
#include "types.h" // TypeCID

using namespace std;

class CInfoWifi : public CCoordinate
{
		TypeCID Cid;

	public :

		void SetCid(TypeCID cid);
		
		TypeCID GetCid();

		void Display(ostream& os) const;

		friend ostream& operator<<(ostream& os, const CInfoWifi& infowifi);
};

#endif
