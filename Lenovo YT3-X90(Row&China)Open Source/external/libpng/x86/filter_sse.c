// Copyright 2015 Intel Inc.

#include "../pngpriv.h"

#ifdef PNG_X86_SSE_OPT

#include <stdint.h>

#ifndef __SSSE3__
#include <emmintrin.h>
#else
#include <tmmintrin.h>
#endif

void
png_read_filter_row_up_sse(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_size_t i;
   __m128i* rp = (__m128i*)(row);
   const __m128i* pp = (__m128i*)(prev_row);

   for (i = (row_info->rowbytes + 15) >> 4; i > 0; i--)
   {
      __m128i rb = _mm_load_si128(rp);
      __m128i prb = _mm_load_si128(pp++);

      rb = _mm_add_epi8(rb, prb);
      _mm_store_si128(rp++, rb);
   }
}

void
png_read_filter_row_sub3_sse(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_size_t i;
   png_bytep rp = row;
   __m128i racc = _mm_setzero_si128();

   PNG_UNUSED(prev_row)

   __m128i nrb = _mm_load_si128((__m128i*)(rp));

   for (i = 0; i < row_info->rowbytes; i += 15, rp += 15)
   {
      __m128i rb = nrb;
#ifndef __SSSE3__
      nrb = _mm_loadu_si128((__m128i*)(rp + 15));
      racc = _mm_srli_si128(_mm_slli_si128(racc, 1), 13);
      racc = _mm_or_si128(racc, _mm_slli_si128(rb, 3));
#else
      nrb = _mm_lddqu_si128((__m128i*)(rp + 15));
      racc =  _mm_alignr_epi8(rb, _mm_slli_si128(racc, 1), 13);
#endif

      rb = _mm_add_epi8(rb, racc);
      racc = _mm_slli_si128(racc, 3);
      rb = _mm_add_epi8(rb, racc);
      racc = _mm_slli_si128(racc, 3);
      rb = _mm_add_epi8(rb, racc);
      racc = _mm_slli_si128(racc, 3);
      rb = _mm_add_epi8(rb, racc);
      racc = _mm_slli_si128(racc, 3);
      rb = _mm_add_epi8(rb, racc);
      racc = rb;

      _mm_storeu_si128((__m128i*)rp, rb);
   }
}

void
png_read_filter_row_sub4_sse(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_size_t i;
   __m128i racc = _mm_setzero_si128();
   __m128i* rp = (__m128i*)(row);

   PNG_UNUSED(prev_row)

   for (i = (row_info->rowbytes + 15) >> 4; i > 0; i--)
   {
      __m128i rb = _mm_load_si128(rp);

#ifndef __SSSE3__
      racc = _mm_srli_si128(racc, 12);
      racc = _mm_or_si128(racc, _mm_slli_si128(rb, 4));
#else
      racc =  _mm_alignr_epi8(rb, racc, 12);
#endif

      rb = _mm_add_epi8(rb, racc);
      racc = _mm_slli_si128(racc, 4);
      rb = _mm_add_epi8(rb, racc);
      racc = _mm_slli_si128(racc, 4);
      rb = _mm_add_epi8(rb, racc);
      racc = _mm_slli_si128(racc, 4);
      rb = _mm_add_epi8(rb, racc);
      racc = rb;

      _mm_store_si128(rp++, rb);
   }
}

static inline __m128i calculate_pixel_avg(const __m128i rb,
   const __m128i prb, __m128i pixel, const __m128i mask)
{
   __m128i round;

   round = _mm_xor_si128(prb, pixel);
   pixel = _mm_avg_epu8(pixel, prb);
   round = _mm_and_si128(round, mask);
   pixel = _mm_sub_epi8(pixel, round);
   return _mm_add_epi8(pixel, rb);
}

void
png_read_filter_row_avg3_sse(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_size_t i;
   png_bytep rp = row;
   png_const_bytep prp = prev_row;
   __m128i nrb = _mm_load_si128((__m128i*)(rp));
   __m128i pixel = _mm_setzero_si128();
   const __m128i mask = _mm_set1_epi8(0x01);

   for (i = 0; i < row_info->rowbytes; i += 15, rp += 15, prp += 15)
   {
#ifndef __SSSE3__
      __m128i prb = _mm_loadu_si128((__m128i*)prp);
#else
      __m128i prb = _mm_lddqu_si128((__m128i*)prp);
#endif
      __m128i rb = nrb;

      // First pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
      prb = _mm_srli_si128(prb, 3);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 3);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 13));
#else
      rb = _mm_alignr_epi8(pixel, rb, 3);
#endif

      // Second pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
      prb = _mm_srli_si128(prb, 3);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 3);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 13));
#else
      rb = _mm_alignr_epi8(pixel, rb, 3);
#endif

      // Third pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
      prb = _mm_srli_si128(prb, 3);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 3);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 13));
#else
      rb = _mm_alignr_epi8(pixel, rb, 3);
#endif

      // Fourth pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
      prb = _mm_srli_si128(prb, 3);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 3);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 13));
#else
      rb = _mm_alignr_epi8(pixel, rb, 3);
#endif

      // Fifth pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
#ifndef __SSSE3__
      nrb = _mm_loadu_si128((__m128i*)(rp + 15));
      rb = _mm_srli_si128(rb, 3);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 13));
#else
      nrb = _mm_lddqu_si128((__m128i*)(rp + 15));
      rb = _mm_alignr_epi8(pixel, rb, 3);
#endif

      rb = _mm_srli_si128(rb, 1);
      _mm_storeu_si128((__m128i*)rp, rb);
   }
}

void
png_read_filter_row_avg4_sse(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_size_t i;
   __m128i* rp = (__m128i*)row;
   const __m128i* prp = (const __m128i*)prev_row;
   __m128i pixel = _mm_setzero_si128();
   const __m128i mask = _mm_set1_epi8(0x01);

   for (i = (row_info->rowbytes + 15) >> 4; i > 0; i--)
   {
      __m128i prb = _mm_load_si128(prp++);
      __m128i rb = _mm_load_si128(rp);

      // First pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
      prb = _mm_srli_si128(prb, 4);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 4);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 12));
#else
      rb = _mm_alignr_epi8(pixel, rb, 4);
#endif

      // Second pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
      prb = _mm_srli_si128(prb, 4);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 4);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 12));
#else
      rb = _mm_alignr_epi8(pixel, rb, 4);
#endif

      // Third pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
      prb = _mm_srli_si128(prb, 4);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 4);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 12));
#else
      rb = _mm_alignr_epi8(pixel, rb, 4);
#endif

      // Fourth pixel
      pixel = calculate_pixel_avg(rb, prb, pixel, mask);
#ifndef __SSSE3__
      rb = _mm_srli_si128(rb, 4);
      rb = _mm_or_si128(rb, _mm_slli_si128(pixel, 12));
#else
      rb = _mm_alignr_epi8(pixel, rb, 4);
#endif

      _mm_store_si128(rp++, rb);
   }
}

void
png_read_filter_row_paeth3_sse(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_size_t i;
   png_bytep rp = row;
   png_const_bytep prp = prev_row;
   __m128i npix = _mm_cvtsi32_si128(*(uint32_t*)rp);
   __m128i ppix = _mm_setzero_si128();           // Same as 'a' in C version.
   __m128i prppix = _mm_setzero_si128();         // Same as 'c' in C version.
   const __m128i zero = _mm_setzero_si128();

   for (i = 0; i < row_info->rowbytes; i += 3, rp += 3, prp += 3)
   {
      __m128i prpix = _mm_cvtsi32_si128(*(uint32_t*)prp);  // Same as 'b' in C ver.
      __m128i pix, pa, pb, pc, temp;

      prpix = _mm_unpacklo_epi8(prpix, zero);
      temp = _mm_sub_epi16(prpix, prppix);  // p = b - c
      pc = _mm_sub_epi16(ppix, prppix);     // pc = a - c

#ifndef __SSSE3__
      pa = _mm_max_epi16(temp, _mm_sub_epi16(prppix, prpix));
      pb = _mm_max_epi16(pc, _mm_sub_epi16(prppix, ppix));
      temp = _mm_add_epi16(temp, pc);
      pc = _mm_max_epi16(temp, _mm_sub_epi16(zero, temp));
#else
      pa = _mm_abs_epi16(temp);             // pa = abs(p)
      pb = _mm_abs_epi16(pc);               // pb = abs(pc)
      temp = _mm_add_epi16(temp, pc);
      pc = _mm_abs_epi16(temp);             // pc = abs(p + pc)
#endif

      temp = _mm_cmplt_epi16(pb, pa);       // if (pb < pa) pa = pb, a = b
      pa = _mm_andnot_si128(temp, pa);
      pa = _mm_or_si128(pa, _mm_and_si128(temp, pb));
      ppix = _mm_andnot_si128(temp, ppix);
      ppix = _mm_or_si128(ppix, _mm_and_si128(temp, prpix));

      pix = npix;
      npix = _mm_cvtsi32_si128(*(uint32_t*)(rp + 3));
      temp = _mm_cmplt_epi16(pc, pa);       // if (pc < pa) a = c
      ppix = _mm_andnot_si128(temp, ppix);
      ppix = _mm_or_si128(ppix, _mm_and_si128(temp, prppix));

      pix = _mm_unpacklo_epi8(pix, zero);
      prppix = prpix;
      ppix = _mm_add_epi16(ppix, pix);

      ppix = _mm_slli_epi16(ppix, 8);
      ppix = _mm_srli_epi16(ppix, 8);
      pix = _mm_packus_epi16(ppix, zero);
      *(uint32_t*)rp = _mm_cvtsi128_si32(pix);
   }
}

void
png_read_filter_row_paeth4_sse(png_row_infop row_info, png_bytep row,
   png_const_bytep prev_row)
{
   png_size_t i;
   uint32_t* rp = (uint32_t*)row;
   const uint32_t* prp = (const uint32_t*)prev_row;
   __m128i ppix = _mm_setzero_si128();           // Same as 'a' in C version.
   __m128i prppix = _mm_setzero_si128();         // Same as 'c' in C version.
   const __m128i zero = _mm_setzero_si128();

   for (i = (row_info->rowbytes + 3) >> 2; i > 0; i--)
   {
      __m128i prpix = _mm_cvtsi32_si128(*prp++); // Same as 'b' in C version.
      __m128i pix, pa, pb, pc, temp;

      prpix = _mm_unpacklo_epi8(prpix, zero);
      temp = _mm_sub_epi16(prpix, prppix);  // p = b - c
      pc = _mm_sub_epi16(ppix, prppix);     // pc = a - c

#ifndef __SSSE3__
      pa = _mm_max_epi16(temp, _mm_sub_epi16(prppix, prpix));
      pb = _mm_max_epi16(pc, _mm_sub_epi16(prppix, ppix));
      temp = _mm_add_epi16(temp, pc);
      pc = _mm_max_epi16(temp, _mm_sub_epi16(zero, temp));
#else
      pa = _mm_abs_epi16(temp);             // pa = abs(p)
      pb = _mm_abs_epi16(pc);               // pb = abs(pc)
      temp = _mm_add_epi16(temp, pc);
      pc = _mm_abs_epi16(temp);             // pc = abs(p + pc)
#endif

      temp = _mm_cmplt_epi16(pb, pa);       // if (pb < pa) pa = pb, a = b
      pa = _mm_andnot_si128(temp, pa);
      pa = _mm_or_si128(pa, _mm_and_si128(temp, pb));
      ppix = _mm_andnot_si128(temp, ppix);
      ppix = _mm_or_si128(ppix, _mm_and_si128(temp, prpix));

      pix = _mm_cvtsi32_si128(*rp);
      temp = _mm_cmplt_epi16(pc, pa);       // if (pc < pa) a = c
      ppix = _mm_andnot_si128(temp, ppix);
      ppix = _mm_or_si128(ppix, _mm_and_si128(temp, prppix));

      pix = _mm_unpacklo_epi8(pix, zero);
      prppix = prpix;
      ppix = _mm_add_epi16(ppix, pix);

      ppix = _mm_slli_epi16(ppix, 8);
      ppix = _mm_srli_epi16(ppix, 8);
      pix = _mm_packus_epi16(ppix, zero);
      *rp++ = _mm_cvtsi128_si32(pix);
   }
}

#endif
