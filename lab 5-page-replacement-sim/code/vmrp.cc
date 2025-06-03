/*
* Filename         ：vmrp.cc
* copyright        ： (C) 2006 by zhonghonglie
* Function         ：模拟虚拟内存页置换算法的程序
*/
#include "vmrp.h"

Replace::Replace() {
	int i;
	// 设定总访问页数，并分配引用页号和淘汰页号记录数组空间
	std::cout << "Please input page numbers: ";
	std::cin >> PageNumber;
	ReferencePage = new int[PageNumber];
	EliminatePage = new int[PageNumber];

	// 输入引用页号序列（页面走向）
	std::cout << "Please input reference page string: ";
	for (i = 0; i < PageNumber; i++) {
		std::cin >> ReferencePage[i];
	}

	// 输入内存实页数（帧数）
	std::cout << "Please input page frames: ";
	std::cin >> FrameNumber;
	PageFrames = new int[FrameNumber];

	// 初始化页帧数组
	for (i = 0; i < FrameNumber; i++) {
		PageFrames[i] = -1;
	}
}

Replace::~Replace() {
	// 释放动态分配的内存
	delete[] ReferencePage;
	delete[] EliminatePage;
	delete[] PageFrames;
}

void Replace::InitSpace(char *MethodName) {
	int i;
	std::cout << std::endl << MethodName << std::endl;
	FaultNumber = 0;

	// 初始化淘汰页数组和页帧数组
	for (i = 0; i < PageNumber; i++) {
		EliminatePage[i] = -1;
	}
	for (i = 0; i < FrameNumber; i++) {
		PageFrames[i] = -1;
	}
}

void Replace::Report(void) {
	// 报告淘汰页顺序
	std::cout << std::endl << "Eliminate page: ";
	for (int i = 0; EliminatePage[i] != -1; i++) {
		std::cout << EliminatePage[i] << " ";
	}

	// 报告缺页数和缺页率
	std::cout << std::endl << "Number of page faults=" << FaultNumber << std::endl;
	std::cout << std::setw(6) << std::setprecision(3);
	std::cout << "Rate of page faults="
			<< 100 * (float)FaultNumber / (float)PageNumber << "%" << std::endl;
}

// 最近最旧未用算法（LRU）
void Replace::Lru(void) {
	int i, j, k, l = 0, next;
	InitSpace(const_cast<char*>("LRU"));

	for (k = 0; k < PageNumber; k++) {
		next = ReferencePage[k];
		// 检查页是否已在内存
		for (i = 0; i < FrameNumber; i++) {
			if (next == PageFrames[i]) {
				// 调整栈顶
				for (j = i; j > 0; j--) {
					PageFrames[j] = PageFrames[j-1];
				}
				PageFrames[0] = next;
				break;
			}
		}

		if (PageFrames[0] == next) {
			// 页已在内存
			for (j = 0; j < FrameNumber; j++) {
				if (PageFrames[j] >= 0) std::cout << PageFrames[j] << " ";
			}
			std::cout << std::endl;
			continue;
		} else {
			// 缺页处理
			FaultNumber++;
			EliminatePage[l] = PageFrames[FrameNumber-1];

			// 压栈操作
			for (j = FrameNumber-1; j > 0; j--) {
				PageFrames[j] = PageFrames[j-1];
			}
			PageFrames[0] = next;
			l++;
		}

		// 输出当前状态
		for (j = 0; j < FrameNumber; j++) {
			if (PageFrames[j] >= 0) std::cout << PageFrames[j] << " ";
		}
		if (EliminatePage[l-1] >= 0) {
			std::cout << "-> " << EliminatePage[l-1];
		}
		std::cout << std::endl;
	}
	Report();
}

// 先进先出算法（FIFO）
void Replace::Fifo(void) {
	int i, j, k, l = 0, next, ptr = 0;
	InitSpace(const_cast<char*>("FIFO"));

	for (k = 0; k < PageNumber; k++) {
		next = ReferencePage[k];
		// 检查页是否已在内存
		for (i = 0; i < FrameNumber; i++) {
			if (next == PageFrames[i]) break;
		}
		if (i < FrameNumber) {
			// 页已在内存
			for (i = 0; i < FrameNumber; i++) {
				std::cout << PageFrames[i] << " ";
			}
			std::cout << std::endl;
			continue;
		}

		// 缺页处理
		FaultNumber++;
		EliminatePage[l] = PageFrames[ptr];
		PageFrames[ptr] = next;
		ptr = (ptr + 1) % FrameNumber;

		// 输出当前状态
		for (i = 0; i < FrameNumber; i++) {
			if (PageFrames[i] >= 0) std::cout << PageFrames[i] << " ";
		}
		if (EliminatePage[l] >= 0) {
			std::cout << "-> " << EliminatePage[l++];
		}
		std::cout << std::endl;
	}
	Report();
}

// 未实现的其他算法框架
void Replace::Clock(void) {}
void Replace::Eclock(void) {}
void Replace::Lfu(void) {}
void Replace::Mfu(void) {}

int main(int argc, char *argv[]) {
	Replace *vmpr = new Replace();
	vmpr->Lru();   // 执行LRU算法
	vmpr->Fifo();  // 执行FIFO算法
	delete vmpr;
	return 0;
}