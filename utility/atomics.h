/*
    Copyright 2011, Spyros Blanas.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MYATOMICS__
#define __MYATOMICS__

/**
 * Atomic compare-and-swap: If the current value of \a *ptr is \a oldval, then
 * write \a newval into \a *ptr.
 * @return the contents of *ptr before the operation. If return value is equal
 * to \a oldval, this means that the operation was successful.
 */
inline void* atomic_compare_and_swap(void** ptr, void* oldval, void* newval)
{
//#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
#if defined(Roanldo)
	newval = __sync_val_compare_and_swap(ptr, oldval, newval);// Remove for compiler error remover
#elif defined(__sparcv9)
	__asm__ __volatile__ ( \
			"casx [%3], %2, %0   \n\t" \
:           "+r" (newval), "+m" (*ptr)     /* output */ \
:           "r" (oldval), "r" (ptr) /* input */ \
:           "memory"    /* clobber */ \
	);
#elif defined(__sparcv8)
	__asm__ __volatile__ ( \
			"cas [%3], %2, %0   \n\t" \
:           "+r" (newval), "+m" (*ptr)     /* output */ \
:           "r" (oldval), "r" (ptr) /* input */ \
:           "memory"    /* clobber */ \
	);
#elif defined(__i386__) 
	__asm__ __volatile__ ( \
			"lock cmpxchgl %2, %1   \n\t" \
:           "=a" (newval), "+m" (*ptr)     /* output */ \
:           "r" (newval), "a" (oldval) /* input */ \
	);
#elif defined(__x86_64__)
	__asm__ __volatile__ ( \
			"lock cmpxchgq %q2, %1   \n\t" \
:           "=a" (newval), "+m" (*ptr)     /* output */ \
:           "r" (newval), "a" (oldval) /* input */ \
	);
#else
#error CAS instruction not known for this architecture.
#endif
	return newval;
}
inline int atomic_compare_and_swap(int* ptr,int oldval,int newval)
{
//#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
#if defined(Roanldo)
	newval = __sync_val_compare_and_swap(ptr, oldval, newval);// Remove for compiler error remover
#elif defined(__x86_64__)
	__asm__ __volatile__ ( \
				"lock cmpxchgq %q2, %1   \n\t" \
	:           "=a" (newval), "+m" (*ptr)     /* output */ \
	:           "r" (newval), "a" (oldval) /* input */ \
		);
#endif
	return newval;
}
inline char* atomic_compare_and_swap(char** ptr,char* oldval,char* newval)
{
//#if defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8) || defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
#if defined(Roanldo)
	newval = __sync_val_compare_and_swap(ptr, oldval, newval);// Remove for compiler error remover
#elif defined(__x86_64__)
	__asm__ __volatile__ ( \
				"lock cmpxchgq %q2, %1   \n\t" \
	:           "=a" (newval), "+m" (*ptr)     /* output */ \
	:           "r" (newval), "a" (oldval) /* input */ \
		);
#endif
	return newval;
}
#endif
