#include "dask.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <cstdlib>
#include <functional>

DiskArm::DiskArm() {
	std::cout << "Please input current Cylinder:";
	std::cin >> CurrentCylinder;
	//磁头⽅向
	// 0 ：向⼩磁道号⽅向移动
	// 1 ：向⼤磁道号⽅向移动
	std::cout << "Please input current direction (0/1):";
	std::cin >> SeekDirection;
	//输入磁盘请求数，请求道号
	std::cout << "Please input request numbers:";
	std::cin >> RequestNumber;
	std::cout << "Please input request cylinder string:";
	Request = new int[RequestNumber];
	Cylinder = new int[RequestNumber];
	for (int i = 0; i < RequestNumber; ++i)
		std::cin >> Request[i];
}

DiskArm::~DiskArm() {
	delete[] Request;
	delete[] Cylinder;
}
//初始化道号，寻道记录

void DiskArm::InitSpace(char *MethodName) {
	std::cout << std::endl << MethodName << std::endl;
	SeekNumber = 0;
	SeekChange = 0;
	for (int i = 0; i < RequestNumber; ++i)
		Cylinder[i] = Request[i];
}
//统计报告算法执⾏情况
void DiskArm::Report() {
	std::cout << std::endl;
	std::cout << "Seek Number:" << SeekNumber << std::endl;
	std::cout << "Change Direction: " << SeekChange << std::endl << std::endl;
}
void DiskArm::FCFS() {
	int Current = CurrentCylinder;
	int Direction = SeekDirection;
	InitSpace(const_cast<char*>("FCFS"));
	std::cout << Current;
	for (int i = 0; i < RequestNumber; ++i) {
		bool needChangeDirection = ((Cylinder[i] < Current) && Direction) ||
		((Cylinder[i] > Current) && !Direction);
		if (needChangeDirection) {
			Direction = !Direction;
			++SeekChange; //调头数加1
			//报告当前响应的道号
			std::cout << std::endl << Current << "->" << Cylinder[i];
		} else {
			//不需要调头，报告当前响应的道号
			std::cout << "->" << Cylinder[i];
		}
			//累计寻道数，响应过的道号变为当前道号
		SeekNumber += abs(Current - Cylinder[i]);
		Current = Cylinder[i];
	}
	Report();
}
//报告磁盘调度情况
void DiskArm::SSTF(){
	int Current = CurrentCylinder;
	int Direction = SeekDirection;
	InitSpace(const_cast<char*>("SSTF"));
	std::cout << Current;

	bool* processed = new bool[RequestNumber];
	for (int i = 0; i < RequestNumber; ++i) {
		processed[i] = false;
	}

	for (int count = 0; count < RequestNumber; ++count) {
		int minDistance = INT_MAX;
		int nextIndex = -1;

		for (int i = 0; i < RequestNumber; ++i) {
			if (!processed[i]) {
				int distance = abs(Current - Cylinder[i]);
				if (distance < minDistance) {
					minDistance = distance;
					nextIndex = i;
				}
			}
		}

		bool needChangeDirection = ((Cylinder[nextIndex] > Current)&&!Direction) || ((Cylinder[nextIndex] < Current) && Direction);
		if (needChangeDirection) {
			Direction = !Direction;
			++SeekChange;
			std::cout << std::endl << Current << "->" << Cylinder[nextIndex];
		} else {
			std::cout << "->" << Cylinder[nextIndex];
		}

		SeekNumber += minDistance;
		Current = Cylinder[nextIndex];
		processed[nextIndex] = true;
	}

	delete[] processed;
	Report();
}

void DiskArm::SCAN() {
	int Current = CurrentCylinder;
	int Direction = SeekDirection;
	InitSpace(const_cast<char*>("SCAN"));
	std::cout << Current;

	int* sortedCylinders = new int[RequestNumber];
	for (int i = 0; i < RequestNumber; ++i) {
		sortedCylinders[i] = Cylinder[i];
	}

	std::sort(sortedCylinders, sortedCylinders + RequestNumber);

	int pos = std::lower_bound(sortedCylinders, sortedCylinders + RequestNumber, CurrentCylinder) - sortedCylinders;

	int count = 0;
	if (Direction) {
		for (int i = pos; i < RequestNumber; ++i) {
			std::cout << "->" << sortedCylinders[i];
			SeekNumber += abs(Current - sortedCylinders[i]);
			Current = sortedCylinders[i];
			++count;
		}

		if (count < RequestNumber) {
			++SeekChange;
			std::cout << std::endl << Current;

			for (int i = pos - 1; i >= 0; --i) {
				std::cout << "->" << sortedCylinders[i];
				SeekNumber += abs(Current - sortedCylinders[i]);
				Current = sortedCylinders[i];
			}
		}
	} else {
		int processedCount = 0;
		for (int i = pos - 1; i >= 0; --i) {
			std::cout << "->" << sortedCylinders[i];
			SeekNumber += abs(Current - sortedCylinders[i]);
			Current = sortedCylinders[i];
			++processedCount;
		}

		if (processedCount < RequestNumber) {
			++SeekChange;
			std::cout << std::endl << Current;

			for (int i = pos; i < RequestNumber; ++i) {
				std::cout << "->" << sortedCylinders[i];
				SeekNumber += abs(Current - sortedCylinders[i]);
				Current = sortedCylinders[i];
			}
		}
	}

	delete[] sortedCylinders;
	Report();
}

void DiskArm::CSCAN() {
	int Current = CurrentCylinder;
	int Direction = SeekDirection;
	InitSpace(const_cast<char*>("CSCAN"));
	std::cout << Current;

	int* sortedCylinders = new int[RequestNumber];
	for (int i = 0; i < RequestNumber; ++i) {
		sortedCylinders[i] = Cylinder[i];
	}

	std::sort(sortedCylinders, sortedCylinders + RequestNumber);

	int pos = std::lower_bound(sortedCylinders, sortedCylinders + RequestNumber, CurrentCylinder) - sortedCylinders;

	if (Direction) {
		for (int i = pos; i < RequestNumber; ++i) {
			std::cout << "->" << sortedCylinders[i];
			SeekNumber += abs(Current - sortedCylinders[i]);
			Current = sortedCylinders[i];
		}

		if (pos > 0) {
			++SeekChange;
			std::cout << std::endl << Current;
			SeekNumber += abs(Current - sortedCylinders[0]);
			Current = sortedCylinders[0];
			std::cout << "->" << Current;

			for (int i = 1; i < pos; ++i) {
				std::cout << "->" << sortedCylinders[i];
				SeekNumber += abs(Current - sortedCylinders[i]);
				Current = sortedCylinders[i];
			}
		}
	} else {
		for (int i = pos - 1; i >= 0; --i) {
			std::cout << "->" << sortedCylinders[i];
			SeekNumber += abs(Current - sortedCylinders[i]);
			Current = sortedCylinders[i];
		}

		if (pos < RequestNumber) {
			++SeekChange;
			std::cout << std::endl << Current;
			SeekNumber += abs(Current - sortedCylinders[RequestNumber - 1]);
			Current = sortedCylinders[RequestNumber - 1];
			std::cout << "->" << Current;

			for (int i = RequestNumber - 2; i >= pos; --i) {
				std::cout << "->" << sortedCylinders[i];
				SeekNumber += abs(Current - sortedCylinders[i]);
				Current = sortedCylinders[i];
			}
		}
	}

	delete[] sortedCylinders;
	Report();
}

void DiskArm::LOOK() {
	int Current = CurrentCylinder;
	int Direction = SeekDirection;
	InitSpace(const_cast<char*>("LOOK"));
	std::cout << Current;

	int* larger = new int[RequestNumber];
	int* smaller = new int[RequestNumber];
	int largerCount = 0, smallerCount = 0;

	for (int i = 0; i < RequestNumber; ++i) {
		if (Cylinder[i] >= Current) {
			larger[largerCount++] = Cylinder[i];
		} else {
			smaller[smallerCount++] = Cylinder[i];
		}
	}

	std::sort(smaller, smaller + smallerCount, std::greater<int>());
	std::sort(larger, larger + largerCount);

	if (Direction) {
		for (int i = 0; i < largerCount; ++i) {
			std::cout << "->" << larger[i];
			SeekNumber += abs(Current - larger[i]);
			Current = larger[i];
		}

		if (smallerCount > 0) {
			++SeekChange;
			std::cout << std::endl << Current;

			for (int i = 0; i < smallerCount; ++i) {
				std::cout << "->" << smaller[i];
				SeekNumber += abs(Current - smaller[i]);
				Current = smaller[i];
			}
		}
	} else {
		for (int i = 0; i < smallerCount; ++i) {
			std::cout << "->" << smaller[i];
			SeekNumber += abs(Current - smaller[i]);
			Current = smaller[i];
		}

		if (largerCount > 0) {
			++SeekChange;
			std::cout << std::endl << Current;

			for (int i = 0; i < largerCount; ++i) {
				std::cout << "->" << larger[i];
				SeekNumber += abs(Current - larger[i]);
				Current = larger[i];
			}
		}
	}

	delete[] larger;
	delete[] smaller;
	Report();
}

int main() {
	DiskArm diskarm;
	diskarm.FCFS();
	diskarm.SSTF();
	diskarm.SCAN();
	diskarm.CSCAN();
	diskarm.LOOK();
	return 0;
}