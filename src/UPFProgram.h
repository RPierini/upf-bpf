#ifndef __UPFPROGRAMMANAGER_H__
#define __UPFPROGRAMMANAGER_H__

#include <ProgramLifeCycle.hpp>
#include <atomic>
#include <linux/bpf.h> // manage maps (e.g. bpf_update*)
#include <memory>
#include <mutex>
#include <signal.h> // signals
#include <upf_xdp_bpf_skel.h>
#include <wrappers/BPFMap.hpp>

class BPFMaps;
class BPFMap;
class SessionManager;
class RulesUtilities;

using UPFProgramLifeCycle = ProgramLifeCycle<upf_xdp_bpf_c>;

/**
 * @brief Singleton class to abrastract the UPF bpf program.
 */
class UPFProgram
{
public:
  /**
   * @brief Construct a new UPFProgram object.
   *
   */
  explicit UPFProgram();
  /**
   * @brief Destroy the UPFProgram object
   */
  virtual ~UPFProgram();
  /**
   * @brief Setup the BPF program.
   *
   */
  void setup();
  /**
   * @brief Get the BPFMaps object.
   *
   * @return std::shared_ptr<BPFMaps> The reference of the BPFMaps.
   */
  std::shared_ptr<BPFMaps> getMaps();
  /**
   * @brief Tear downs the BPF program.
   *
   */
  void tearDown();
  /**
   * @brief Update program int map.
   *
   * @param key The key which will be inserted the program file descriptor.
   * @param fd The file descriptor.
   */
  void updateProgramMap(uint32_t key, uint32_t fd);
  /**
   * @brief Remove program in map.
   *
   * @param key The key which will be remove in the program map.
   */
  void removeProgramMap(uint32_t key);
  /**
   * @brief Get the Programs Map object.
   *
   * @return std::shared_ptr<BPFMap> The seid to fd map.
   */
  std::shared_ptr<BPFMap> getProgramsMap() const;

private:
  /**
   * @brief Initialize BPF wrappers maps.
   *
   */
  void initializeMaps();

  // The reference of the bpf maps.
  std::shared_ptr<BPFMaps> mpMaps;

  // The skeleton of the UPF program generated by bpftool.
  // ProgramLifeCycle is the owner of the pointer.
  upf_xdp_bpf_c *spSkeleton;

  // The program eBPF map.
  std::shared_ptr<BPFMap> mpProgramsMap;

  // The BPF lifecycle program.
  std::unique_ptr<UPFProgramLifeCycle> mpLifeCycle;
};

#endif // __BPFPROGRAMMANAGER_H__
