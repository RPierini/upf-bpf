#include "SessionProgram.h"
#include <UPFProgram.h>
#include <bpf/bpf.h>          // bpf calls
#include <bpf/libbpf.h>       // bpf wrappers
#include <sys/resource.h>     // rlimit
#include <utils/LogDefines.h> // Logs
#include <net/if.h>           // if_nametoindex
#include <wrappers/BPFMaps.h>
#include <wrappers/BPFMap.hpp>

SessionProgram::SessionProgram() 
{
  LOG_FUNC();
  mpLifeCycle = std::make_unique<SessionProgramLifeCycle>(session_bpf_c__open, session_bpf_c__load, session_bpf_c__attach, session_bpf_c__destroy);
}

SessionProgram::~SessionProgram()
{
  LOG_FUNC();
}

void SessionProgram::setup()
{
  LOG_FUNC();

  mpLifeCycle->open();
  initializeMaps();
  mpLifeCycle->load();
  mpLifeCycle->attach();

  auto egressInterface = if_nametoindex("veth0");

  // mpMaps->getMap("m_id_txcnt").update(key_ifmap, sXDPProgramInfo[1].ifIndex, 0);
 
}

void SessionProgram::tearDown() 
{
  LOG_FUNC();
  mpLifeCycle->destroy();
}

int SessionProgram::getFileDescriptor() const
{
  LOG_FUNC();
  return bpf_program__fd(mpLifeCycle->getBPFSkeleton()->progs.entry_point);
}

std::shared_ptr<BPFMap> SessionProgram::getPDRMap() const
{
  LOG_FUNC();
  return mpPDRMap;
}

std::shared_ptr<BPFMap> SessionProgram::getFARMap() const
{
  LOG_FUNC();
  return mpFARMap;
}

std::shared_ptr<BPFMap> SessionProgram::getUplinkPDRsMap() const
{
  LOG_FUNC();
  return mpUplinkPDRsMap;
}

std::shared_ptr<BPFMap> SessionProgram::getCounterMap() const
{
  LOG_FUNC();
  return mpCounterMap;
}

void SessionProgram::initializeMaps()
{
  LOG_FUNC();
  // Store all maps available in the program.
  mpMaps = std::make_unique<BPFMaps>(mpLifeCycle->getBPFSkeleton()->skeleton);

  // Warning - The name of the map must be the same of the BPF program.
  // Initialize maps.
  mpPDRMap = std::make_shared<BPFMap>(mpMaps->getMap("m_pdrs"));
  mpFARMap = std::make_shared<BPFMap>(mpMaps->getMap("m_fars"));
  mpUplinkPDRsMap = std::make_shared<BPFMap>(mpMaps->getMap("m_teid_pdr"));
  mpCounterMap = std::make_shared<BPFMap>(mpMaps->getMap("mc_stats"));
  mpEgressInterfaceMap = std::make_shared<BPFMap>(mpMaps->getMap("m_redirect_interfaces"));
}