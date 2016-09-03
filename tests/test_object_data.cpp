#include <boost/test/unit_test.hpp>
#include <data/ModelData.hpp>
#include "test_globals.hpp"

BOOST_AUTO_TEST_SUITE(ObjectDataTests)

#if RW_TEST_WITH_DATA
BOOST_AUTO_TEST_CASE(test_object_data)
{
	{
		LoaderIDE l;

		l.load(Global::get().getGamePath() + "/data/maps/generic.ide");

		BOOST_ASSERT( l.objects.find(1100) != l.objects.end() );

		auto obj = l.objects[1100];

    auto def = std::dynamic_pointer_cast<SimpleModelData>(obj);

    BOOST_ASSERT(def->type == ModelDataType::SimpleInfo);

		BOOST_CHECK_EQUAL( def->modelName, "rd_Corner1" );
		BOOST_CHECK_EQUAL( def->textureName, "generic" );
		BOOST_CHECK_EQUAL( def->numClumps, 1 );
		BOOST_CHECK_EQUAL( def->drawDistance[0], 220 );
		BOOST_CHECK_EQUAL( def->flags, 0 );
	}
	{
		LoaderIDE l;

		l.load(Global::get().getGamePath() + "/data/default.ide");

		BOOST_ASSERT( l.objects.find(90) != l.objects.end() );

		auto obj = l.objects[90];

    auto def = std::dynamic_pointer_cast<VehicleModelData>(obj);

    BOOST_ASSERT(def->type == ModelDataType::VehicleInfo);

		BOOST_CHECK_EQUAL( def->modelName, "landstal");
		BOOST_CHECK_EQUAL( def->textureName, "landstal" );
    BOOST_CHECK_EQUAL( def->vehicletype, VehicleModelData::CAR );
		BOOST_CHECK_EQUAL( def->handlingID, "LANDSTAL" );
		BOOST_CHECK_EQUAL( def->gameName, "LANDSTK" );
    BOOST_CHECK_EQUAL( def->classType, VehicleModelData::RICHFAMILY );
		BOOST_CHECK_EQUAL( def->frequency, 10 );
		BOOST_CHECK_EQUAL( def->wheelModelID, 164 );
		BOOST_CHECK_CLOSE( def->wheelScale, 0.8f, 0.01f);
	}
}

BOOST_AUTO_TEST_CASE(test_gamedata_data)
{
	
}
#endif

BOOST_AUTO_TEST_SUITE_END()
