
/*******************************************************************************
* FILENAME
*   type.h
*
* DESCRIPTION
*   c641x Header File
*
*******************************************************************************/

#ifndef _TYPE_H_
#define _TYPE_H_



typedef char int8;                        /**< The 8-bit signed data type. */

typedef volatile char vint8;              /**< The volatile 8-bit signed data type. */
                                           
typedef unsigned char uint8;              /**< The 8-bit unsigned data type. */
                                           
typedef volatile unsigned char vuint8;    /**< The volatile 8-bit unsigned data type. */
                                           
typedef short int16;                      /**< The 16-bit signed data type. */
                                           
typedef volatile short vint16;            /**< The volatile 16-bit signed data type. */
                                           
typedef unsigned short uint16;            /**< The 16-bit unsigned data type. */
                                           
typedef volatile unsigned short vuint16;  /**< The volatile 16-bit unsigned data type. */
                                           
typedef int int32;                        /**< The 32-bit signed data type. */
                                           
typedef volatile int vint32;              /**< The volatile 32-bit signed data type. */
                                           
typedef unsigned int uint32;              /**< The 32-bit unsigned data type. */
                                           
typedef volatile unsigned int vuint32;    /**< The volatile 32-bit unsigned data type. */


typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned char	uchar;
typedef unsigned int    uint;

#ifndef __cplusplus
typedef int				bool;
#define	true			1
#define false			0
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#endif	
