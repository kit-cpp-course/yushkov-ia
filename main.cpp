#include <list>
#include "ISorterEngine.hpp"
#include "ConsoleLogger.hpp"
#include "RandomDataProvider.hpp"

using namespace wv;

int main()
{
	const auto dataProvider = new RandomDataProvider<int>(100, 1, 1000);
	const auto logger = new ConsoleLogger();

	const auto singleThreadSorter = new SingleThreadSorter<int>(logger);
	const auto multiThreadSorter = new MultiThreadSorter<int>(logger);

	multiThreadSorter->Sort(*dataProvider);
	logger->LogLine("");
	singleThreadSorter->Sort(*dataProvider);

	delete dataProvider;
	delete logger;
	delete singleThreadSorter;
	delete multiThreadSorter;
}