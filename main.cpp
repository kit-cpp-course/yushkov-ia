#include <list>
#include "ISorterEngine.hpp"
#include "ConsoleLogger.hpp"
#include "RandomDataProvider.hpp"

using namespace wv;

int main()
{
	const IDataProvider<int> & dataProvider = RandomDataProvider<int>(100, 1, 1000);
	const ILogger & logger = ConsoleLogger();

	const ISorterEngine<int> & singleThreadSorter = SingleThreadSorter<int>(&logger);
	const ISorterEngine<int> & multiThreadSorter = MultiThreadSorter<int>(&logger);

	multiThreadSorter.Sort(dataProvider);
	logger.LogLine("");
	singleThreadSorter.Sort(dataProvider);
}