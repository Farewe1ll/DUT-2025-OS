#include <vector>
#include <chrono>
#include <random>
#include "vmrp.h"

using namespace std;

Replace::Replace() {
	int i;
	// 设定总访问页数，并分配引用页号和淘汰页号记录数组空间
	cout << "Please input page numbers: ";
	cin >> PageNumber;
	ReferencePage.resize(PageNumber);
	EliminatePage.assign(PageNumber, -1);

	// 输入引用页号序列（页面走向）
	cout << "Please input reference page string: ";
	for (i = 0; i < PageNumber; ++i) {
		cin >> ReferencePage[i];
	}

	// 输入内存实页数（帧数）
	cout << "Please input page frames: ";
	cin >> FrameNumber;

	PageFrames.assign(FrameNumber, -1);
}

void Replace::GenerateRandomSequence(int totalPages, int sequenceLength) {
	mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
	PageNumber = sequenceLength;
	ReferencePage.resize(PageNumber);
	for (int i = 0; i < PageNumber; ++i) {
		ReferencePage[i] = rnd() % totalPages + 1;
	}
}

Replace::~Replace() {}

void Replace::InitSpace(char *MethodName) {
	cout << endl << MethodName << endl;
	FaultNumber = 0;
	// 初始化淘汰页数组和页帧数组
	fill(EliminatePage.begin(), EliminatePage.end(), -1);
	fill(PageFrames.begin(), PageFrames.end(), -1);
}

void Replace::Report(void) {
	// 报告淘汰页顺序
	cout << endl << "Eliminate page: ";
	for (int i = 0; EliminatePage[i] != -1; ++i) {
		cout << EliminatePage[i] << " ";
	}

	// 报告缺页数和缺页率
	cout << endl << "Number of page faults=" << FaultNumber << endl;
	cout << std::setw(6) << std::setprecision(3);
	cout << "Rate of page faults="
			<< 100 * (float)FaultNumber / (float)PageNumber << "%" << endl;
}

// 最近最旧未用算法（LRU）
void Replace::Lru(void) {
	int i, j, k, l = 0, nxt;
	InitSpace(const_cast<char*>("LRU"));

	for (k = 0; k < PageNumber; ++k) {
		nxt = ReferencePage[k];
		// 检查页是否已在内存
		for (i = 0; i < FrameNumber; ++i) {
			if (nxt == PageFrames[i]) {
				// 调整栈顶
				for (j = i; j > 0; j--) {
					PageFrames[j] = PageFrames[j-1];
				}
				PageFrames[0] = nxt;
				break;
			}
		}

		if (PageFrames[0] == nxt) {
			// 页已在内存
			for (j = 0; j < FrameNumber; ++j) {
				if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
			}
			cout << endl;
			continue;
		} else {
			// 缺页处理
			FaultNumber++;
			EliminatePage[l] = PageFrames[FrameNumber-1];

			// 压栈操作
			for (j = FrameNumber-1; j > 0; j--) {
				PageFrames[j] = PageFrames[j-1];
			}
			PageFrames[0] = nxt;
			l++;
		}

		// 输出当前状态
		for (j = 0; j < FrameNumber; ++j) {
			if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
		}
		if (EliminatePage[l-1] >= 0) {
			cout << "-> " << EliminatePage[l-1];
		}
		cout << endl;
	}
	Report();
}

// 先进先出算法（FIFO）
void Replace::Fifo(void) {
	int i, k, l = 0, nxt, ptr = 0;
	InitSpace(const_cast<char*>("FIFO"));

	for (k = 0; k < PageNumber; ++k) {
		nxt = ReferencePage[k];
		// 检查页是否已在内存
		for (i = 0; i < FrameNumber; ++i) {
			if (nxt == PageFrames[i]) break;
		}
		if (i < FrameNumber) {
			// 页已在内存
			for (i = 0; i < FrameNumber; ++i) {
				cout << PageFrames[i] << " ";
			}
			cout << endl;
			continue;
		}

		// 缺页处理
		FaultNumber++;
		EliminatePage[l] = PageFrames[ptr];
		PageFrames[ptr] = nxt;
		ptr = (ptr + 1) % FrameNumber;

		// 输出当前状态
		for (i = 0; i < FrameNumber; ++i) {
			if (PageFrames[i] >= 0) cout << PageFrames[i] << " ";
		}
		if (EliminatePage[l] >= 0) {
			cout << "-> " << EliminatePage[l++];
		} else {
			cout << endl;
		}
	}
	Report();
}

// 未实现的其他算法框架
void Replace::Clock(void) {
	int i, j, k, nxt, cnt = 0;
	InitSpace(const_cast<char*>("CLOCK"));
	vector<int> refBit(FrameNumber, 0);
	int ptr = 0;

	for (k = 0; k < PageNumber; ++k) {
		nxt = ReferencePage[k];

		for (i = 0; i < FrameNumber; ++i) {
			if (nxt == PageFrames[i]) {
				refBit[i] = 1;
				break;
			}
		}

		if (i < FrameNumber) {
			for (j = 0; j < FrameNumber; ++j)
				if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
			cout << endl;
			continue;
		}

		++FaultNumber;

		for (; ;) {
			if (refBit[ptr] == 0) break;
			refBit[ptr] = 0;
			ptr = (ptr + 1) % FrameNumber;
		}

		EliminatePage[cnt] = PageFrames[ptr];
		PageFrames[ptr] = nxt;
		refBit[ptr] = 1;
		ptr = (ptr + 1) % FrameNumber;

		for (j = 0; j < FrameNumber; ++j) {
			if (PageFrames[j] >= 0) {
				cout << PageFrames[j] << " ";
			}
		}
		if (EliminatePage[cnt] >= 0) {
			cout << "-> " << EliminatePage[cnt++] << endl;
		} else {
			cout << endl;
		}
	}

	Report();
}
void Replace::Eclock(void) {
	InitSpace(const_cast<char*>("ECLOCK"));

	int i, j, nxt, cnt = 0;
	vector<int> refBit(FrameNumber, 0), modBit(FrameNumber, 0);
	int ptr = 0;

	for (int k = 0; k < PageNumber; ++k) {
		nxt = ReferencePage[k];

		for(i = 0; i < FrameNumber; ++i) {
			if(nxt == PageFrames[i]) {
				refBit[i] = 1;
				// if (modify) modBit[i] = 1;
				break;
			}
		}

		if (i < FrameNumber) {
			for (j = 0; j < FrameNumber; ++j) {
				if (PageFrames[j] >= 0) {
					cout << PageFrames[j] << " ";
				}
			}
			cout << endl;
			continue;
		}

		++FaultNumber;

		bool found = 0;
		for (i = 0; i < FrameNumber; ++i) {
			if (PageFrames[i] == -1) {
				ptr = i;
				found = 1;
				break;
			}
		}
		for (int p = (ptr + 1) % FrameNumber; p != ptr && !found ; ++p, p %= FrameNumber) {
			if (refBit[p] == 0 && modBit[p] == 0) {
				found = 1;
				ptr = p;
				break;
			}
		}
		for (int p = (ptr + 1) % FrameNumber; p != ptr && !found ; ++p, p %= FrameNumber) {
			if (refBit[p] == 0 && modBit[p] == 1) {
				found = 1;
				ptr = p;
				break;
			}
			refBit[p] = 0;
		}
		for (int p = (ptr + 1) % FrameNumber; p != ptr && !found ; ++p, p %= FrameNumber) {
			if (refBit[p] == 0 && modBit[p] == 0) {
				found = 1;
				ptr = p;
				break;
			}
		}
		for (int p = (ptr + 1) % FrameNumber; p != ptr && !found ; ++p, p %= FrameNumber) {
			if (refBit[p] == 0 && modBit[p] == 1) {
				found = 1;
				ptr = p;
				break;
			}
		}

		EliminatePage[cnt] = PageFrames[ptr];

		PageFrames[ptr] = nxt;
		refBit[ptr] = 1;
		modBit[ptr] = 0;

		ptr = (ptr + 1) % FrameNumber;

		for (j = 0; j < FrameNumber; ++j) {
			if (PageFrames[j] >= 0) {
				cout << PageFrames[j] << " ";
			}
		}
		if (EliminatePage[cnt] >= 0) {
			cout << "-> " << EliminatePage[cnt++] << endl;
		} else {
			cout << endl;
		}
	}

	Report();
}
void Replace::Lfu(void) {
	int i, j, nxt, cnt = 0;
	InitSpace(const_cast<char*>("LFU"));
	vector<int> freqCount(FrameNumber, 0);

	for (int k = 0; k < PageNumber; ++k) {
		nxt = ReferencePage[k];

		for (i = 0; i < FrameNumber; ++i) {
			if (nxt == PageFrames[i]) {
				++freqCount[i];
				break;
			}
		}

		if (i < FrameNumber) {
			for (j = 0; j < FrameNumber; ++j)
				if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
			cout << endl;
			continue;
		}

		++FaultNumber;

		int minIndex = 0;
		for (i = 1; i < FrameNumber; ++i) {
			if (freqCount[i] < freqCount[minIndex]) {
				minIndex = i;
			}
		}

		EliminatePage[cnt] = PageFrames[minIndex];
		PageFrames[minIndex] = nxt;
		freqCount[minIndex] = 1;

		for (j = 0; j < FrameNumber; ++j) {
			if (PageFrames[j] >= 0) {
				cout << PageFrames[j] << " ";
			}
		}
		if (EliminatePage[cnt] >= 0) {
			cout << "-> " << EliminatePage[cnt++] << endl;
		} else {
			cout << endl;
		}
	}

	Report();
}
void Replace::Mfu(void) {
	int i, j, nxt, cnt = 0;
	InitSpace(const_cast<char*>("MFU"));
	vector<int> freqCount(FrameNumber, 0);

	for (int k = 0; k < PageNumber; ++k) {
		nxt = ReferencePage[k];

		for (i = 0; i < FrameNumber; ++i) {
			if (nxt == PageFrames[i]) {
				++freqCount[i];
				break;
			}
		}

		if (i < FrameNumber) {
			for (j = 0; j < FrameNumber; ++j) {
				if (PageFrames[j] >= 0) {
					cout << PageFrames[j] << " ";
				}
			}
			cout << endl;
			continue;
		}

		++FaultNumber;

		int maxIndex = -1;
		for (i = 0; i < FrameNumber; ++i) {
			if (PageFrames[i] == -1) {
				maxIndex = i;
				break;
			}
		}

		if (maxIndex == -1) {
			maxIndex = 0;
			for (i = 1; i < FrameNumber; ++i) {
				if (freqCount[i] > freqCount[maxIndex]) {
					maxIndex = i;
				}
			}
		}

		EliminatePage[cnt] = PageFrames[maxIndex];
		PageFrames[maxIndex] = nxt;
		freqCount[maxIndex] = 1;

		for (j = 0; j < FrameNumber; ++j) {
			if (PageFrames[j] >= 0) cout << PageFrames[j] << " ";
		}
		if (EliminatePage[cnt] >= 0) {
			cout << "-> " << EliminatePage[cnt++] << endl;
		} else {
			cout << endl;
		}
	}

	Report();
}

int main(int argc, char *argv[]) {
	Replace *vmpr = new Replace();
	vmpr->Lru();
	vmpr->Fifo();
	vmpr->Clock();
	vmpr->Eclock();
	vmpr->Lfu();
	vmpr->Mfu();
	delete vmpr;
	return 0;
}