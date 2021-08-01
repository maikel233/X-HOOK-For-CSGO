#pragma once

#include "../sdk/SDK.h"
#include "../interfaces.h"
#include <deque>

namespace Backtrack
{
    void update(ClientFrameStage_t) noexcept;
    void run(CUserCmd*) noexcept;
	void AddLatencyToNetwork(NetworkChannel*, float) noexcept;
    void UpdateIncomingSequences(bool reset = false) noexcept;

    struct Record {
        Vector max;
        Vector mins;
      //  StudioHdr* hdr;
        Vector origin;
        float simulationTime;
        matrix3x4_t matrix[256];
        Vector headPos;
    };

    std::deque<Record>* getRecords(std::size_t index) noexcept;
    bool valid(float simtime) noexcept;

    struct IncomingSequence
    {
        int inreliablestate;
        int sequencenr;
        float servertime;
    };
    extern std::deque<IncomingSequence>sequences;

    float getExtraTicks() noexcept;
    void init() noexcept;
}
