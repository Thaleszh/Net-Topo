/**
 * \addtogroup CkLdb
*/
/*@{*/

#ifndef _MYNEIGHBORLB_H_
#define _MYNEIGHBORLB_H_

#include <math.h>

#include "myNborBaseLB.h"
#include "myNeighborLB.decl.h"
// last try: act as if net_topo is a module

void CreatemyNeighborLB();

class myNeighborLB : public CBase_myNeighborLB {
public:
  myNeighborLB(const CkLBOptions &);
  myNeighborLB(CkMigrateMessage *m):CBase_myNeighborLB(m) {}
private:
  bool QueryBalanceNow(int step) { return true; };
  virtual int max_neighbors() {
    return (CkNumPes() > 5) ? 4 : (CkNumPes()-1);
  };
  virtual int num_neighbors() {
    return (CkNumPes() > 5) ? 4 : (CkNumPes()-1);
  };
  virtual void neighbors(int* _n) {
    const int me = CkMyPe();
    const int npe = CkNumPes();
    if (npe > 1)
      _n[0] = (me + npe - 1) % npe;
    if (npe > 2)
      _n[1] = (me + 1) % npe;

    int bigstep = (npe - 1) / 3 + 1;
    if (bigstep == 1) bigstep++;

    if (npe > 3)
      _n[2] = (me + bigstep) % npe;
    if (npe > 4)
      _n[3] = (me + npe - bigstep) % npe;
    if (_lb_args.debug()>1)
      CkPrintf("Neighbors: [%d], [%d], [%d], [%d] !\n", _n[1], _n[2], _n[3], _n[4]);
  };

  LBMigrateMsg* Strategy(myNborBaseLB::LDStats* stats, int n_nbrs);
};

#endif /* _myNeighborLB_H_ */

/*@}*/
