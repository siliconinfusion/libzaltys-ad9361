/******************************************************************************

  Author        : Paul Onions
  Creation date : 20 August 2019

  COMMERCIAL IN CONFIDENCE
  (C) 2019 Silicon Infusion Limited

  Silicon Infusion Limited                 
  CP House
  Otterspool Way
  Watford WD25 8HP
  Hertfordshire, UK
  Tel: +44 (0)1923 650404
  Fax: +44 (0)1923 650374
  Web: www.siliconinfusion.com

  This is an unpublished work the copyright of which vests in Silicon Infusion
  Limited. All rights reserved. The information contained herein is the
  property of Silicon Infusion Limited and is supplied without liability for
  errors or omissions. No part may be reproduced or used except as authorised
  by contract or other written permission. The copyright and the foregoing
  restriction on reproduction and use extend to all media in which the
  information may be embodied.

*******************************************************************************
  Debug utilities.

******************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#ifdef TESTING
#define DBGPRINTF(str,...) {printf(str,__VA_ARGS__);}
#else
#define DBGPRINTF(str,...) {}
#endif

#endif /* DEBUG_H */
