#include "../Source/NodeContainer.h"
#include <gtest/gtest.h>

struct NodeContainerTests : testing::Test
{
	NodeContainer* nodeContainer;

	NodeContainerTests()
	{
		nodeContainer = new NodeContainer();
	}

	~NodeContainerTests()
	{
		delete nodeContainer;
	}
};