// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_EcomSubsystem.h"

void UEIK_EcomSubsystem::EIK_Ecom_CatalogItem_Release(FEIK_Ecom_CatalogItem CatalogItem)
{
	EOS_Ecom_CatalogItem_Release(&CatalogItem.Ref);
}
