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

#ifndef __MYLOCK__
#define __MYLOCK__

/**
 * Non-recursive spinlock. Using `xchg` and `ldstub` as in PostgresSQL.
 */
class SpineLock {
	public:
	SpineLock() : _l(0) { }

		/** Call blocks and retunrs only when it has the lock. */
		inline void lock() 
		{
			while(tas(&_l)) {
#if defined(__i386__) || defined(__x86_64__)
				__asm__ __volatile__ ("pause\n");
#endif
			}
		}
		inline bool try_lock()
		{
			return !tas(&_l);
		}
		inline int fakelock()
		{
			return _l==0?0:1;
		}
		inline int getvalue()
		{
			//return _l==0?0:1;
			return 0;
		}

		/** Unlocks the lock object. */
		inline void unlock() 
		{ 
			_l = 0;
		}

	private:
		inline int tas(volatile char* lock)
		{
			register char res = 1;
#if defined(__i386__) || defined(__x86_64__)
			__asm__ __volatile__ (
					"lock xchgb %0, %1\n"
					: "+q"(res), "+m"(*lock)
					:
					: "memory", "cc");
#elif defined(__sparc__)
			__asm__ __volatile__ (
					"ldstub [%2], %0"
					: "=r"(res), "+m"(*lock)
					: "r"(lock)
					: "memory");
#else
#error TAS not defined for this architecture.
#endif
			return res;
		}

		volatile char _l ;//__attribute__((aligned(64)));

};

#endif
