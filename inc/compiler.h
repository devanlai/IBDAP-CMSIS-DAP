/*
 * compiler.h
 *
 *  Created on: Jun 18, 2015
 *      Author: yliu
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#if defined ( __GNUC__ )
#define __ALWAYS_INLINE __attribute__((always_inline))
#define __forceinline	__ALWAYS_INLINE
#define __weak			__attribute__ ((weak))
#endif



#endif /* COMPILER_H_ */
