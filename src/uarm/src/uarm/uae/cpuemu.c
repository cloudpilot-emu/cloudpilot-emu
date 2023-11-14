#include "UAE.h"

#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wsign-compare"

#if !defined(PART_1) && !defined(PART_2) && !defined(PART_3) &&                \
    !defined(PART_4) && !defined(PART_5) && !defined(PART_6) &&                \
    !defined(PART_7) && !defined(PART_8)
#define PART_1 1
#define PART_2 1
#define PART_3 1
#define PART_4 1
#define PART_5 1
#define PART_6 1
#define PART_7 1
#define PART_8 1
#endif

#ifdef PART_1
unsigned long REGPARAM2 op_0_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_10_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_18_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_20_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_28_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_30_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_38_3(uae_u32 opcode) /* OR */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_39_3(uae_u32 opcode) /* OR */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_3c_3(uae_u32 opcode) /* ORSR */
{
  {
    MakeSR();
    {
      uae_s16 src = get_iword(2);
      src &= 0xFF;
      regs.sr |= src;
      MakeFromSR();
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_40_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_50_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_58_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_60_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_68_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_70_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_78_3(uae_u32 opcode) /* OR */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_79_3(uae_u32 opcode) /* OR */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_80_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_90_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_98_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_a0_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_a8_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_b0_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_b8_3(uae_u32 opcode) /* OR */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_b9_3(uae_u32 opcode) /* OR */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_ilong(6);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(10);
  return 18;
}
unsigned long REGPARAM2 op_100_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_108_3(uae_u32 opcode) /* MVPMR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr memp = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
    {
      uae_u16 val = (get_byte(memp) << 8) + get_byte(memp + 2);
      m68k_dreg(regs, dstreg) =
          (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_110_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_118_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_120_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_128_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_130_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_138_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_139_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_13a_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 2;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_getpc() + 2;
        dsta += (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_13b_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 3;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr tmppc = m68k_getpc() + 2;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_13c_3(uae_u32 opcode) /* BTST */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = get_ibyte(2);
        src &= 7;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_140_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        dst ^= (1 << src);
        SET_ZFLG((dst >> src) & 1);
        m68k_dreg(regs, dstreg) = (dst);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_148_3(uae_u32 opcode) /* MVPMR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr memp = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
    {
      uae_u32 val = (get_byte(memp) << 24) + (get_byte(memp + 2) << 16) +
                    (get_byte(memp + 4) << 8) + get_byte(memp + 6);
      m68k_dreg(regs, dstreg) = (val);
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_150_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_158_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_160_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_168_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_170_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_178_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_179_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_17a_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 2;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_getpc() + 2;
        dsta += (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_17b_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 3;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr tmppc = m68k_getpc() + 2;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_180_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
        dst &= ~(1 << src);
        m68k_dreg(regs, dstreg) = (dst);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_188_3(uae_u32 opcode) /* MVPRM */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      uaecptr memp = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
      put_byte(memp, src >> 8);
      put_byte(memp + 2, src);
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_190_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_198_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1a0_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1a8_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1b0_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1b8_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1b9_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_1ba_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 2;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_getpc() + 2;
        dsta += (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1bb_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 3;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr tmppc = m68k_getpc() + 2;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1c0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
        dst |= (1 << src);
        m68k_dreg(regs, dstreg) = (dst);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_1c8_3(uae_u32 opcode) /* MVPRM */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      uaecptr memp = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
      put_byte(memp, src >> 24);
      put_byte(memp + 2, src >> 16);
      put_byte(memp + 4, src >> 8);
      put_byte(memp + 6, src);
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_1d0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1d8_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1e0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1e8_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1f0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1f8_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1f9_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_1fa_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 2;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_getpc() + 2;
        dsta += (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1fb_3(uae_u32 opcode) /* BSET */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = 3;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr tmppc = m68k_getpc() + 2;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_200_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_210_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_218_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_220_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_228_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_230_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_238_3(uae_u32 opcode) /* AND */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_239_3(uae_u32 opcode) /* AND */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_23c_3(uae_u32 opcode) /* ANDSR */
{
  {
    MakeSR();
    {
      uae_s16 src = get_iword(2);
      src |= 0xFF00;
      regs.sr &= src;
      MakeFromSR();
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_240_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_250_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_258_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_260_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_268_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_270_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_278_3(uae_u32 opcode) /* AND */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_279_3(uae_u32 opcode) /* AND */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_280_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_290_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_298_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_2a0_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_2a8_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_2b0_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_2b8_3(uae_u32 opcode) /* AND */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_2b9_3(uae_u32 opcode) /* AND */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_ilong(6);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(10);
  return 18;
}
unsigned long REGPARAM2 op_400_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_410_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_418_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_420_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_428_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_430_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_438_3(uae_u32 opcode) /* SUB */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_439_3(uae_u32 opcode) /* SUB */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_440_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_450_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_458_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_460_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_468_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_470_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_478_3(uae_u32 opcode) /* SUB */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_479_3(uae_u32 opcode) /* SUB */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_480_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_490_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_498_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_4a0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_4a8_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_4b0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_4b8_3(uae_u32 opcode) /* SUB */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_4b9_3(uae_u32 opcode) /* SUB */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_ilong(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(10);
  return 18;
}
unsigned long REGPARAM2 op_600_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_610_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_618_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_620_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_628_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_630_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_638_3(uae_u32 opcode) /* ADD */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_639_3(uae_u32 opcode) /* ADD */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_640_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_650_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_658_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_660_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_668_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_670_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_678_3(uae_u32 opcode) /* ADD */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_679_3(uae_u32 opcode) /* ADD */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_680_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_690_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_698_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_6a0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_6a8_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_6b0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_6b8_3(uae_u32 opcode) /* ADD */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_6b9_3(uae_u32 opcode) /* ADD */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_ilong(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(10);
  return 18;
}
unsigned long REGPARAM2 op_800_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_810_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_818_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_820_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_828_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_830_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_838_3(uae_u32 opcode) /* BTST */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_839_3(uae_u32 opcode) /* BTST */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(8);
  return 10;
}
unsigned long REGPARAM2 op_83a_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = 2;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_getpc() + 4;
        dsta += (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_83b_3(uae_u32 opcode) /* BTST */
{
  uae_u32 dstreg = 3;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr tmppc = m68k_getpc() + 4;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_83c_3(uae_u32 opcode) /* BTST */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s8 dst = get_ibyte(4);
        src &= 7;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_840_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        dst ^= (1 << src);
        SET_ZFLG((dst >> src) & 1);
        m68k_dreg(regs, dstreg) = (dst);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_850_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_858_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_860_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_868_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_870_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_878_3(uae_u32 opcode) /* BCHG */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_879_3(uae_u32 opcode) /* BCHG */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_87a_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = 2;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_getpc() + 4;
        dsta += (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_87b_3(uae_u32 opcode) /* BCHG */
{
  uae_u32 dstreg = 3;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr tmppc = m68k_getpc() + 4;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          dst ^= (1 << src);
          SET_ZFLG((dst >> src) & 1);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_880_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
        dst &= ~(1 << src);
        m68k_dreg(regs, dstreg) = (dst);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_890_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_898_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8a0_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8a8_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8b0_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8b8_3(uae_u32 opcode) /* BCLR */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8b9_3(uae_u32 opcode) /* BCLR */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_8ba_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = 2;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_getpc() + 4;
        dsta += (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8bb_3(uae_u32 opcode) /* BCLR */
{
  uae_u32 dstreg = 3;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr tmppc = m68k_getpc() + 4;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst &= ~(1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8c0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= 31;
        SET_ZFLG(1 ^ ((dst >> src) & 1));
        dst |= (1 << src);
        m68k_dreg(regs, dstreg) = (dst);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_8d0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8d8_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8e0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8e8_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8f0_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8f8_3(uae_u32 opcode) /* BSET */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8f9_3(uae_u32 opcode) /* BSET */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_8fa_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = 2;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_getpc() + 4;
        dsta += (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8fb_3(uae_u32 opcode) /* BSET */
{
  uae_u32 dstreg = 3;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr tmppc = m68k_getpc() + 4;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src &= 7;
          SET_ZFLG(1 ^ ((dst >> src) & 1));
          dst |= (1 << src);
          put_byte(dsta, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_a00_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src ^= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_a10_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_a18_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_a20_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_a28_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_a30_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_a38_3(uae_u32 opcode) /* EOR */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_a39_3(uae_u32 opcode) /* EOR */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_a3c_3(uae_u32 opcode) /* EORSR */
{
  {
    MakeSR();
    {
      uae_s16 src = get_iword(2);
      src &= 0xFF;
      regs.sr ^= src;
      MakeFromSR();
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_a40_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src ^= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_a50_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_a58_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_a60_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_a68_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_a70_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
#endif

#ifdef PART_2
unsigned long REGPARAM2 op_a78_3(uae_u32 opcode) /* EOR */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_a79_3(uae_u32 opcode) /* EOR */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_a80_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src ^= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_a90_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_a98_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_aa0_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_aa8_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_ab0_3(uae_u32 opcode) /* EOR */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_ab8_3(uae_u32 opcode) /* EOR */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_ab9_3(uae_u32 opcode) /* EOR */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_ilong(6);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(10);
  return 18;
}
unsigned long REGPARAM2 op_c00_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_c10_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c18_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c20_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c28_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c30_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c38_3(uae_u32 opcode) /* CMP */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c39_3(uae_u32 opcode) /* CMP */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 10;
}
unsigned long REGPARAM2 op_c3a_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = 2;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_getpc() + 4;
        dsta += (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c3b_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = 3;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr tmppc = m68k_getpc() + 4;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(4));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c40_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_c50_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c58_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c60_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c68_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c70_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c78_3(uae_u32 opcode) /* CMP */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c79_3(uae_u32 opcode) /* CMP */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 10;
}
unsigned long REGPARAM2 op_c7a_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = 2;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_getpc() + 4;
        dsta += (uae_s32)(uae_s16)get_iword(4);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c7b_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = 3;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr tmppc = m68k_getpc() + 4;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(4));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c80_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_c90_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_c98_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_ca0_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_ca8_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_cb0_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_cb8_3(uae_u32 opcode) /* CMP */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_cb9_3(uae_u32 opcode) /* CMP */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_ilong(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(10);
  return 14;
}
unsigned long REGPARAM2 op_cba_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = 2;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_getpc() + 6;
        dsta += (uae_s32)(uae_s16)get_iword(6);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_cbb_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = 3;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr tmppc = m68k_getpc() + 6;
        uaecptr dsta = get_disp_ea_000(tmppc, get_iword(6));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_1000_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_1010_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_1018_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_1020_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_1028_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_1030_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_1038_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_1039_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_103a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_103b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_103c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_1080_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_1090_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1098_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_10a0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_10a8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10b0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10b8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10b9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_10ba_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10bb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10bc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_10c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_10d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_10d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_10e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_10e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10f8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10f9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_10fa_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10fb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_10fc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_1100_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_1110_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1118_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1120_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_1128_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1130_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1138_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1139_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_113a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_113b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_113c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_1140_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_1150_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1158_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1160_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1168_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_1170_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_1178_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_1179_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_117a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_117b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_117c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_1180_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_1190_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_1198_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_11a0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_11a8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11b0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11b8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11b9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_11ba_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11bb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11bc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_11c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_11d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_11d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_11e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_11e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11f8_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11f9_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_11fa_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11fb_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_11fc_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_13c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_13d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_13d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_13e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_13e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_13f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_13f8_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_13f9_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_ilong(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(10);
  return 14;
}
unsigned long REGPARAM2 op_13fa_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_13fb_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_13fc_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uaecptr dsta = get_ilong(4);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        put_byte(dsta, src);
      }
    }
  }
  m68k_incpc(8);
  return 10;
}
unsigned long REGPARAM2 op_2000_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_2008_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_2010_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2018_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2020_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2028_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2030_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2038_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2039_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_203a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_203b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_203c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_2040_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 val = src;
        m68k_areg(regs, dstreg) = (val);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_2048_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_u32 val = src;
        m68k_areg(regs, dstreg) = (val);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_2050_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2058_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2060_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2068_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2070_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2078_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2079_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_207a_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_207b_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 val = src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_207c_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_u32 val = src;
        m68k_areg(regs, dstreg) = (val);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_2080_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2088_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2090_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_2098_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_20a0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_20a8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20b0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20b8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20b9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_20ba_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20bb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20bc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_20c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += 4;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_20c8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += 4;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_20d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_20d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_20e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_20e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20f8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20f9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_20fa_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20fb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 4;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_20fc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += 4;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_2100_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2108_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_2110_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_2118_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_2120_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_2128_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_2130_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_2138_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_2139_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_213a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_213b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
#endif

#ifdef PART_3
unsigned long REGPARAM2 op_213c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_2140_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2148_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2150_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_2158_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_2160_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_2168_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_2170_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_2178_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_2179_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_217a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_217b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_217c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_2180_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2188_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_2190_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_2198_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_21a0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_21a8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21b0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21b8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21b9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_21ba_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21bb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21bc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_21c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_21c8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_21d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_21d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_21e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_21e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21f8_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21f9_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_21fa_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21fb_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_21fc_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_23c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_23c8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_23d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_23d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_23e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_23e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_23f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_23f8_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_23f9_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_ilong(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(10);
  return 18;
}
unsigned long REGPARAM2 op_23fa_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_23fb_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 16;
}
unsigned long REGPARAM2 op_23fc_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uaecptr dsta = get_ilong(6);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        put_long(dsta, src);
      }
    }
  }
  m68k_incpc(10);
  return 14;
}
unsigned long REGPARAM2 op_3000_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_3008_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_3010_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3018_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3020_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3028_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3030_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3038_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3039_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_303a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_303b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_303c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_3040_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 val = (uae_s32)(uae_s16)src;
        m68k_areg(regs, dstreg) = (val);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_3048_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uae_u32 val = (uae_s32)(uae_s16)src;
        m68k_areg(regs, dstreg) = (val);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_3050_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3058_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3060_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3068_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3070_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3078_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3079_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_307a_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_307b_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 val = (uae_s32)(uae_s16)src;
          m68k_areg(regs, dstreg) = (val);
          if (dstreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_307c_3(uae_u32 opcode) /* MOVEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_u32 val = (uae_s32)(uae_s16)src;
        m68k_areg(regs, dstreg) = (val);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_3080_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3088_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3090_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_3098_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_30a0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_30a8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30b0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30b8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30b9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_30ba_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30bb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30bc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_30c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += 2;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_30c8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += 2;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_30d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_30d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_30e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_30e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30f8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30f9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_30fa_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30fb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) += 2;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_30fc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        m68k_areg(regs, dstreg) += 2;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3100_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3108_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_3110_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_3118_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_3120_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_3128_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_3130_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_3138_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_3139_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_313a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_313b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_313c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        m68k_areg(regs, dstreg) = dsta;
        if (dstreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3140_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3148_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3150_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_3158_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_3160_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_3168_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_3170_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_3178_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_3179_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_317a_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_317b_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta =
              m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_317c_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_3180_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3188_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_3190_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_3198_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_31a0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_31a8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31b0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31b8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31b9_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(6));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_31ba_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31bb_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31bc_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_31c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_31c8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_31d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_31d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_31e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_31e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31f8_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31f9_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_31fa_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31fb_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_31fc_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(4);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_33c0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_33c8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_33d0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_33d8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_33e0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = get_ilong(2);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_33e8_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_33f0_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_33f8_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_33f9_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_ilong(6);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(10);
  return 14;
}
unsigned long REGPARAM2 op_33fa_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_33fb_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uaecptr dsta = get_ilong(4);
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(8);
  return 12;
}
unsigned long REGPARAM2 op_33fc_3(uae_u32 opcode) /* MOVE */
{
  {
    {
      uae_s16 src = get_iword(2);
      {
        uaecptr dsta = get_ilong(4);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        put_word(dsta, src);
      }
    }
  }
  m68k_incpc(8);
  return 10;
}
unsigned long REGPARAM2 op_4000_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
        {
          int flgs = ((uae_s8)(src)) < 0;
          int flgo = ((uae_s8)(0)) < 0;
          int flgn = ((uae_s8)(newv)) < 0;
          SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
          SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
          COPY_CARRY;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xff) | ((newv) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4010_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            put_byte(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4018_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            put_byte(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4020_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            put_byte(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4028_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            put_byte(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4030_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            put_byte(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4038_3(uae_u32 opcode) /* NEGX */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            put_byte(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4039_3(uae_u32 opcode) /* NEGX */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            put_byte(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4040_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
        {
          int flgs = ((uae_s16)(src)) < 0;
          int flgo = ((uae_s16)(0)) < 0;
          int flgn = ((uae_s16)(newv)) < 0;
          SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
          SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
          COPY_CARRY;
          SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
          SET_NFLG(((uae_s16)(newv)) < 0);
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | ((newv) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4050_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            put_word(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4058_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            put_word(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4060_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            put_word(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4068_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            put_word(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4070_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            put_word(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4078_3(uae_u32 opcode) /* NEGX */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            put_word(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4079_3(uae_u32 opcode) /* NEGX */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            put_word(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4080_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
        {
          int flgs = ((uae_s32)(src)) < 0;
          int flgo = ((uae_s32)(0)) < 0;
          int flgn = ((uae_s32)(newv)) < 0;
          SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
          SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
          COPY_CARRY;
          SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
          SET_NFLG(((uae_s32)(newv)) < 0);
          m68k_dreg(regs, srcreg) = (newv);
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4090_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            put_long(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_4098_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            put_long(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_40a0_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            put_long(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_40a8_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            put_long(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_40b0_3(uae_u32 opcode) /* NEGX */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            put_long(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_40b8_3(uae_u32 opcode) /* NEGX */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            put_long(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_40b9_3(uae_u32 opcode) /* NEGX */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 newv = 0 - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            put_long(srca, newv);
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_40c0_3(uae_u32 opcode) /* MVSR2 */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      MakeSR();
      m68k_dreg(regs, srcreg) =
          (m68k_dreg(regs, srcreg) & ~0xffff) | ((regs.sr) & 0xffff);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_40d0_3(uae_u32 opcode) /* MVSR2 */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      MakeSR();
      put_word(srca, regs.sr);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_40d8_3(uae_u32 opcode) /* MVSR2 */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += 2;
      MakeSR();
      put_word(srca, regs.sr);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_40e0_3(uae_u32 opcode) /* MVSR2 */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      MakeSR();
      put_word(srca, regs.sr);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_40e8_3(uae_u32 opcode) /* MVSR2 */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      MakeSR();
      put_word(srca, regs.sr);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_40f0_3(uae_u32 opcode) /* MVSR2 */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      MakeSR();
      put_word(srca, regs.sr);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_40f8_3(uae_u32 opcode) /* MVSR2 */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      MakeSR();
      put_word(srca, regs.sr);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_40f9_3(uae_u32 opcode) /* MVSR2 */
{
  {
    {
      uaecptr srca = get_ilong(2);
      MakeSR();
      put_word(srca, regs.sr);
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_4100_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        if ((uae_s32)dst < 0) {
          SET_NFLG(1);
          Exception(6, oldpc);
          goto endlabel579;
        } else if (dst > src) {
          SET_NFLG(0);
          Exception(6, oldpc);
          goto endlabel579;
        }
      }
    }
  }
  m68k_incpc(2);
endlabel579:;
  return 2;
}
unsigned long REGPARAM2 op_4110_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel580;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel580;
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel580:;
  return 6;
}
unsigned long REGPARAM2 op_4118_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel581;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel581;
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel581:;
  return 6;
}
unsigned long REGPARAM2 op_4120_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel582;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel582;
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel582:;
  return 6;
}
unsigned long REGPARAM2 op_4128_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel583;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel583;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel583:;
  return 8;
}
unsigned long REGPARAM2 op_4130_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel584;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel584;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel584:;
  return 8;
}
unsigned long REGPARAM2 op_4138_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel585;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel585;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel585:;
  return 8;
}
unsigned long REGPARAM2 op_4139_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel586;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel586;
          }
        }
      }
    }
  }
  m68k_incpc(6);
endlabel586:;
  return 10;
}
unsigned long REGPARAM2 op_413a_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel587;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel587;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel587:;
  return 8;
}
unsigned long REGPARAM2 op_413b_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel588;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel588;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel588:;
  return 8;
}
unsigned long REGPARAM2 op_413c_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        if ((uae_s32)dst < 0) {
          SET_NFLG(1);
          Exception(6, oldpc);
          goto endlabel589;
        } else if (dst > src) {
          SET_NFLG(0);
          Exception(6, oldpc);
          goto endlabel589;
        }
      }
    }
  }
  m68k_incpc(6);
endlabel589:;
  return 6;
}
unsigned long REGPARAM2 op_4180_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        if ((uae_s32)dst < 0) {
          SET_NFLG(1);
          Exception(6, oldpc);
          goto endlabel590;
        } else if (dst > src) {
          SET_NFLG(0);
          Exception(6, oldpc);
          goto endlabel590;
        }
      }
    }
  }
  m68k_incpc(2);
endlabel590:;
  return 2;
}
unsigned long REGPARAM2 op_4190_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel591;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel591;
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel591:;
  return 4;
}
unsigned long REGPARAM2 op_4198_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel592;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel592;
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel592:;
  return 4;
}
unsigned long REGPARAM2 op_41a0_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel593;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel593;
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel593:;
  return 4;
}
unsigned long REGPARAM2 op_41a8_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel594;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel594;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel594:;
  return 6;
}
unsigned long REGPARAM2 op_41b0_3(uae_u32 opcode) /* CHK */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel595;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel595;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel595:;
  return 6;
}
unsigned long REGPARAM2 op_41b8_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel596;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel596;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel596:;
  return 6;
}
unsigned long REGPARAM2 op_41b9_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel597;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel597;
          }
        }
      }
    }
  }
  m68k_incpc(6);
endlabel597:;
  return 8;
}
unsigned long REGPARAM2 op_41ba_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel598;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel598;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel598:;
  return 6;
}
unsigned long REGPARAM2 op_41bb_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          if ((uae_s32)dst < 0) {
            SET_NFLG(1);
            Exception(6, oldpc);
            goto endlabel599;
          } else if (dst > src) {
            SET_NFLG(0);
            Exception(6, oldpc);
            goto endlabel599;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel599:;
  return 6;
}
unsigned long REGPARAM2 op_41bc_3(uae_u32 opcode) /* CHK */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        if ((uae_s32)dst < 0) {
          SET_NFLG(1);
          Exception(6, oldpc);
          goto endlabel600;
        } else if (dst > src) {
          SET_NFLG(0);
          Exception(6, oldpc);
          goto endlabel600;
        }
      }
    }
  }
  m68k_incpc(4);
endlabel600:;
  return 4;
}
unsigned long REGPARAM2 op_41d0_3(uae_u32 opcode) /* LEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        {
          uaecptr prev_dstreg = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) = (srca);
          if (dstreg == 7) {
            if (srcreg == 7) {
              if (prev_dstreg < (srca))
                CHECK_STACK_POINTER_INCREMENT();
              if (prev_dstreg > (srca))
                CHECK_STACK_POINTER_DECREMENT();
            } else {
              CHECK_STACK_POINTER_ASSIGNMENT();
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_41e8_3(uae_u32 opcode) /* LEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        {
          uaecptr prev_dstreg = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) = (srca);
          if (dstreg == 7) {
            if (srcreg == 7) {
              if (prev_dstreg < (srca))
                CHECK_STACK_POINTER_INCREMENT();
              if (prev_dstreg > (srca))
                CHECK_STACK_POINTER_DECREMENT();
            } else {
              CHECK_STACK_POINTER_ASSIGNMENT();
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_41f0_3(uae_u32 opcode) /* LEA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        {
          uaecptr prev_dstreg = m68k_areg(regs, dstreg);
          m68k_areg(regs, dstreg) = (srca);
          if (dstreg == 7) {
            if (srcreg == 7) {
              if (prev_dstreg < (srca))
                CHECK_STACK_POINTER_INCREMENT();
              if (prev_dstreg > (srca))
                CHECK_STACK_POINTER_DECREMENT();
            } else {
              CHECK_STACK_POINTER_ASSIGNMENT();
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_41f8_3(uae_u32 opcode) /* LEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        m68k_areg(regs, dstreg) = (srca);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_41f9_3(uae_u32 opcode) /* LEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        m68k_areg(regs, dstreg) = (srca);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_41fa_3(uae_u32 opcode) /* LEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        m68k_areg(regs, dstreg) = (srca);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_41fb_3(uae_u32 opcode) /* LEA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        m68k_areg(regs, dstreg) = (srca);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4200_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      m68k_dreg(regs, srcreg) =
          (m68k_dreg(regs, srcreg) & ~0xff) | ((0) & 0xff);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4210_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      put_byte(srca, 0);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4218_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      put_byte(srca, 0);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4220_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      put_byte(srca, 0);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4228_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      put_byte(srca, 0);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4230_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      put_byte(srca, 0);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4238_3(uae_u32 opcode) /* CLR */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      put_byte(srca, 0);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4239_3(uae_u32 opcode) /* CLR */
{
  {
    {
      uaecptr srca = get_ilong(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(0)) == 0);
      SET_NFLG(((uae_s8)(0)) < 0);
      put_byte(srca, 0);
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_4240_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      m68k_dreg(regs, srcreg) =
          (m68k_dreg(regs, srcreg) & ~0xffff) | ((0) & 0xffff);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4250_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      put_word(srca, 0);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4258_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += 2;
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      put_word(srca, 0);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4260_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      put_word(srca, 0);
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4268_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      put_word(srca, 0);
    }
  }
  m68k_incpc(4);
  return 6;
}
#endif

#ifdef PART_4
unsigned long REGPARAM2 op_4270_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      put_word(srca, 0);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4278_3(uae_u32 opcode) /* CLR */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      put_word(srca, 0);
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4279_3(uae_u32 opcode) /* CLR */
{
  {
    {
      uaecptr srca = get_ilong(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(0)) == 0);
      SET_NFLG(((uae_s16)(0)) < 0);
      put_word(srca, 0);
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_4280_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      m68k_dreg(regs, srcreg) = (0);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4290_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      put_long(srca, 0);
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4298_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += 4;
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      put_long(srca, 0);
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_42a0_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      put_long(srca, 0);
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_42a8_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      put_long(srca, 0);
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_42b0_3(uae_u32 opcode) /* CLR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      put_long(srca, 0);
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_42b8_3(uae_u32 opcode) /* CLR */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      put_long(srca, 0);
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_42b9_3(uae_u32 opcode) /* CLR */
{
  {
    {
      uaecptr srca = get_ilong(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(0)) == 0);
      SET_NFLG(((uae_s32)(0)) < 0);
      put_long(srca, 0);
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4400_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        {
          uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(0)) < 0;
            int flgn = ((uae_s8)(dst)) < 0;
            SET_ZFLG(((uae_s8)(dst)) == 0);
            SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
            SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
            COPY_CARRY;
            SET_NFLG(flgn != 0);
            m68k_dreg(regs, srcreg) =
                (m68k_dreg(regs, srcreg) & ~0xff) | ((dst) & 0xff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4410_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          {
            uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(0)) < 0;
              int flgn = ((uae_s8)(dst)) < 0;
              SET_ZFLG(((uae_s8)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_byte(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4418_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          {
            uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(0)) < 0;
              int flgn = ((uae_s8)(dst)) < 0;
              SET_ZFLG(((uae_s8)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_byte(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4420_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          {
            uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(0)) < 0;
              int flgn = ((uae_s8)(dst)) < 0;
              SET_ZFLG(((uae_s8)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_byte(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4428_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          {
            uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(0)) < 0;
              int flgn = ((uae_s8)(dst)) < 0;
              SET_ZFLG(((uae_s8)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_byte(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4430_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          {
            uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(0)) < 0;
              int flgn = ((uae_s8)(dst)) < 0;
              SET_ZFLG(((uae_s8)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_byte(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4438_3(uae_u32 opcode) /* NEG */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          {
            uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(0)) < 0;
              int flgn = ((uae_s8)(dst)) < 0;
              SET_ZFLG(((uae_s8)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_byte(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4439_3(uae_u32 opcode) /* NEG */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          {
            uae_u32 dst = ((uae_s8)(0)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(0)) < 0;
              int flgn = ((uae_s8)(dst)) < 0;
              SET_ZFLG(((uae_s8)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_byte(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4440_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        {
          uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(0)) < 0;
            int flgn = ((uae_s16)(dst)) < 0;
            SET_ZFLG(((uae_s16)(dst)) == 0);
            SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
            SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
            COPY_CARRY;
            SET_NFLG(flgn != 0);
            m68k_dreg(regs, srcreg) =
                (m68k_dreg(regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4450_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          {
            uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(0)) < 0;
              int flgn = ((uae_s16)(dst)) < 0;
              SET_ZFLG(((uae_s16)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_word(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4458_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          {
            uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(0)) < 0;
              int flgn = ((uae_s16)(dst)) < 0;
              SET_ZFLG(((uae_s16)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_word(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4460_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          {
            uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(0)) < 0;
              int flgn = ((uae_s16)(dst)) < 0;
              SET_ZFLG(((uae_s16)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_word(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4468_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          {
            uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(0)) < 0;
              int flgn = ((uae_s16)(dst)) < 0;
              SET_ZFLG(((uae_s16)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_word(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4470_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          {
            uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(0)) < 0;
              int flgn = ((uae_s16)(dst)) < 0;
              SET_ZFLG(((uae_s16)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_word(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4478_3(uae_u32 opcode) /* NEG */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          {
            uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(0)) < 0;
              int flgn = ((uae_s16)(dst)) < 0;
              SET_ZFLG(((uae_s16)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_word(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4479_3(uae_u32 opcode) /* NEG */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          {
            uae_u32 dst = ((uae_s16)(0)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(0)) < 0;
              int flgn = ((uae_s16)(dst)) < 0;
              SET_ZFLG(((uae_s16)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_word(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4480_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        {
          uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(0)) < 0;
            int flgn = ((uae_s32)(dst)) < 0;
            SET_ZFLG(((uae_s32)(dst)) == 0);
            SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
            SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
            COPY_CARRY;
            SET_NFLG(flgn != 0);
            m68k_dreg(regs, srcreg) = (dst);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4490_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          {
            uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(0)) < 0;
              int flgn = ((uae_s32)(dst)) < 0;
              SET_ZFLG(((uae_s32)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_long(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_4498_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          {
            uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(0)) < 0;
              int flgn = ((uae_s32)(dst)) < 0;
              SET_ZFLG(((uae_s32)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_long(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_44a0_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          {
            uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(0)) < 0;
              int flgn = ((uae_s32)(dst)) < 0;
              SET_ZFLG(((uae_s32)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_long(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_44a8_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          {
            uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(0)) < 0;
              int flgn = ((uae_s32)(dst)) < 0;
              SET_ZFLG(((uae_s32)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_long(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_44b0_3(uae_u32 opcode) /* NEG */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          {
            uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(0)) < 0;
              int flgn = ((uae_s32)(dst)) < 0;
              SET_ZFLG(((uae_s32)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_long(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_44b8_3(uae_u32 opcode) /* NEG */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          {
            uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(0)) < 0;
              int flgn = ((uae_s32)(dst)) < 0;
              SET_ZFLG(((uae_s32)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_long(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_44b9_3(uae_u32 opcode) /* NEG */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          {
            uae_u32 dst = ((uae_s32)(0)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(0)) < 0;
              int flgn = ((uae_s32)(dst)) < 0;
              SET_ZFLG(((uae_s32)(dst)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(0)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              put_long(srca, dst);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_44c0_3(uae_u32 opcode) /* MV2SR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      MakeSR();
      regs.sr &= 0xFF00;
      regs.sr |= src & 0xFF;
      MakeFromSR();
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_44d0_3(uae_u32 opcode) /* MV2SR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_44d8_3(uae_u32 opcode) /* MV2SR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_44e0_3(uae_u32 opcode) /* MV2SR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_44e8_3(uae_u32 opcode) /* MV2SR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_44f0_3(uae_u32 opcode) /* MV2SR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_44f8_3(uae_u32 opcode) /* MV2SR */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_44f9_3(uae_u32 opcode) /* MV2SR */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_44fa_3(uae_u32 opcode) /* MV2SR */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_44fb_3(uae_u32 opcode) /* MV2SR */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        MakeSR();
        regs.sr &= 0xFF00;
        regs.sr |= src & 0xFF;
        MakeFromSR();
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_44fc_3(uae_u32 opcode) /* MV2SR */
{
  {
    {
      uae_s16 src = get_iword(2);
      MakeSR();
      regs.sr &= 0xFF00;
      regs.sr |= src & 0xFF;
      MakeFromSR();
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4600_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 dst = ~src;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(dst)) == 0);
        SET_NFLG(((uae_s8)(dst)) < 0);
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((dst) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4610_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(dst)) == 0);
          SET_NFLG(((uae_s8)(dst)) < 0);
          put_byte(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4618_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(dst)) == 0);
          SET_NFLG(((uae_s8)(dst)) < 0);
          put_byte(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4620_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(dst)) == 0);
          SET_NFLG(((uae_s8)(dst)) < 0);
          put_byte(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4628_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(dst)) == 0);
          SET_NFLG(((uae_s8)(dst)) < 0);
          put_byte(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4630_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(dst)) == 0);
          SET_NFLG(((uae_s8)(dst)) < 0);
          put_byte(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4638_3(uae_u32 opcode) /* NOT */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(dst)) == 0);
          SET_NFLG(((uae_s8)(dst)) < 0);
          put_byte(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4639_3(uae_u32 opcode) /* NOT */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(dst)) == 0);
          SET_NFLG(((uae_s8)(dst)) < 0);
          put_byte(srca, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4640_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 dst = ~src;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(dst)) == 0);
        SET_NFLG(((uae_s16)(dst)) < 0);
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4650_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(dst)) == 0);
          SET_NFLG(((uae_s16)(dst)) < 0);
          put_word(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4658_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(dst)) == 0);
          SET_NFLG(((uae_s16)(dst)) < 0);
          put_word(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4660_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(dst)) == 0);
          SET_NFLG(((uae_s16)(dst)) < 0);
          put_word(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4668_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(dst)) == 0);
          SET_NFLG(((uae_s16)(dst)) < 0);
          put_word(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4670_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(dst)) == 0);
          SET_NFLG(((uae_s16)(dst)) < 0);
          put_word(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4678_3(uae_u32 opcode) /* NOT */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(dst)) == 0);
          SET_NFLG(((uae_s16)(dst)) < 0);
          put_word(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4679_3(uae_u32 opcode) /* NOT */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(dst)) == 0);
          SET_NFLG(((uae_s16)(dst)) < 0);
          put_word(srca, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4680_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 dst = ~src;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(dst)) == 0);
        SET_NFLG(((uae_s32)(dst)) < 0);
        m68k_dreg(regs, srcreg) = (dst);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4690_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(dst)) == 0);
          SET_NFLG(((uae_s32)(dst)) < 0);
          put_long(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_4698_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(dst)) == 0);
          SET_NFLG(((uae_s32)(dst)) < 0);
          put_long(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_46a0_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(dst)) == 0);
          SET_NFLG(((uae_s32)(dst)) < 0);
          put_long(srca, dst);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_46a8_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(dst)) == 0);
          SET_NFLG(((uae_s32)(dst)) < 0);
          put_long(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_46b0_3(uae_u32 opcode) /* NOT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(dst)) == 0);
          SET_NFLG(((uae_s32)(dst)) < 0);
          put_long(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_46b8_3(uae_u32 opcode) /* NOT */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(dst)) == 0);
          SET_NFLG(((uae_s32)(dst)) < 0);
          put_long(srca, dst);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_46b9_3(uae_u32 opcode) /* NOT */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_u32 dst = ~src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(dst)) == 0);
          SET_NFLG(((uae_s32)(dst)) < 0);
          put_long(srca, dst);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_4800_3(uae_u32 opcode) /* NBCD */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
        uae_u16 newv_hi = -(src & 0xF0);
        uae_u16 newv;
        int cflg;
        if (newv_lo > 9) {
          newv_lo -= 6;
          newv_hi -= 0x10;
        }
        newv = newv_hi + (newv_lo & 0xF);
        SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
        COPY_CARRY;
        if (cflg)
          newv -= 0x60;
        SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
        SET_NFLG(((uae_s8)(newv)) < 0);
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((newv) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4810_3(uae_u32 opcode) /* NBCD */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = -(src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          put_byte(srca, newv);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4818_3(uae_u32 opcode) /* NBCD */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = -(src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          put_byte(srca, newv);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4820_3(uae_u32 opcode) /* NBCD */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = -(src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          put_byte(srca, newv);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4828_3(uae_u32 opcode) /* NBCD */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = -(src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          put_byte(srca, newv);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4830_3(uae_u32 opcode) /* NBCD */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = -(src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          put_byte(srca, newv);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4838_3(uae_u32 opcode) /* NBCD */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = -(src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          put_byte(srca, newv);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4839_3(uae_u32 opcode) /* NBCD */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_u16 newv_lo = -(src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = -(src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          put_byte(srca, newv);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4840_3(uae_u32 opcode) /* SWAP */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 dst = ((src >> 16) & 0xFFFF) | ((src & 0xFFFF) << 16);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(dst)) == 0);
        SET_NFLG(((uae_s32)(dst)) < 0);
        m68k_dreg(regs, srcreg) = (dst);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4850_3(uae_u32 opcode) /* PEA */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, 7) - 4;
        m68k_areg(regs, 7) = dsta;
        if (7 == 7)
          CHECK_STACK_POINTER_DECREMENT();
        put_long(dsta, srca);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4868_3(uae_u32 opcode) /* PEA */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, 7) - 4;
        m68k_areg(regs, 7) = dsta;
        if (7 == 7)
          CHECK_STACK_POINTER_DECREMENT();
        put_long(dsta, srca);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4870_3(uae_u32 opcode) /* PEA */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uaecptr dsta = m68k_areg(regs, 7) - 4;
        m68k_areg(regs, 7) = dsta;
        if (7 == 7)
          CHECK_STACK_POINTER_DECREMENT();
        put_long(dsta, srca);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4878_3(uae_u32 opcode) /* PEA */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, 7) - 4;
        m68k_areg(regs, 7) = dsta;
        if (7 == 7)
          CHECK_STACK_POINTER_DECREMENT();
        put_long(dsta, srca);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4879_3(uae_u32 opcode) /* PEA */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uaecptr dsta = m68k_areg(regs, 7) - 4;
        m68k_areg(regs, 7) = dsta;
        if (7 == 7)
          CHECK_STACK_POINTER_DECREMENT();
        put_long(dsta, srca);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_487a_3(uae_u32 opcode) /* PEA */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uaecptr dsta = m68k_areg(regs, 7) - 4;
        m68k_areg(regs, 7) = dsta;
        if (7 == 7)
          CHECK_STACK_POINTER_DECREMENT();
        put_long(dsta, srca);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_487b_3(uae_u32 opcode) /* PEA */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uaecptr dsta = m68k_areg(regs, 7) - 4;
        m68k_areg(regs, 7) = dsta;
        if (7 == 7)
          CHECK_STACK_POINTER_DECREMENT();
        put_long(dsta, srca);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4880_3(uae_u32 opcode) /* EXT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_u16 dst = (uae_s16)(uae_s8)src;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(dst)) == 0);
        SET_NFLG(((uae_s16)(dst)) < 0);
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xffff) | ((dst) & 0xffff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4890_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = m68k_areg(regs, dstreg);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_word(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_word(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_48a0_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = m68k_areg(regs, dstreg) - 0;
      {
        uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
        m68k_areg(regs, dstreg) -= 16 * 2;
        while (amask) {
          srca -= 2;
          put_word(srca, m68k_areg(regs, movem_index2[amask]));
          amask = movem_next[amask];
        }
        while (dmask) {
          srca -= 2;
          put_word(srca, m68k_dreg(regs, movem_index2[dmask]));
          dmask = movem_next[dmask];
        }
        m68k_areg(regs, dstreg) = srca;
        CHECK_STACK_POINTER_DECREMENT();
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_48a8_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_word(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_word(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_48b0_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_word(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_word(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_48b8_3(uae_u32 opcode) /* MVMLE */
{
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(4);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_word(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_word(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_48b9_3(uae_u32 opcode) /* MVMLE */
{
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = get_ilong(4);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_word(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_word(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(8);
  return 8;
}
unsigned long REGPARAM2 op_48c0_3(uae_u32 opcode) /* EXT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 dst = (uae_s32)(uae_s16)src;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(dst)) == 0);
        SET_NFLG(((uae_s32)(dst)) < 0);
        m68k_dreg(regs, srcreg) = (dst);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_48d0_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = m68k_areg(regs, dstreg);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_long(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_long(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_48e0_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = m68k_areg(regs, dstreg) - 0;
      {
        uae_u16 amask = mask & 0xff, dmask = (mask >> 8) & 0xff;
        m68k_areg(regs, dstreg) -= 16 * 4;
        while (amask) {
          srca -= 4;
          put_long(srca, m68k_areg(regs, movem_index2[amask]));
          amask = movem_next[amask];
        }
        while (dmask) {
          srca -= 4;
          put_long(srca, m68k_dreg(regs, movem_index2[dmask]));
          dmask = movem_next[dmask];
        }
        m68k_areg(regs, dstreg) = srca;
        CHECK_STACK_POINTER_DECREMENT();
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_48e8_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_long(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_long(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_48f0_3(uae_u32 opcode) /* MVMLE */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_long(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_long(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_48f8_3(uae_u32 opcode) /* MVMLE */
{
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(4);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_long(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_long(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_48f9_3(uae_u32 opcode) /* MVMLE */
{
  {
    uae_u16 mask = get_iword(2);
    {
      uaecptr srca = get_ilong(4);
      {
        uae_u16 dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
        while (dmask) {
          put_long(srca, m68k_dreg(regs, movem_index1[dmask]));
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          put_long(srca, m68k_areg(regs, movem_index1[amask]));
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(8);
  return 8;
}
unsigned long REGPARAM2 op_49c0_3(uae_u32 opcode) /* EXT */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_u32 dst = (uae_s32)(uae_s8)src;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(dst)) == 0);
        SET_NFLG(((uae_s32)(dst)) < 0);
        m68k_dreg(regs, srcreg) = (dst);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4a00_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(src)) == 0);
      SET_NFLG(((uae_s8)(src)) < 0);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4a10_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4a18_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4a20_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4a28_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a30_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a38_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a39_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_4a3a_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a3b_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a3c_3(uae_u32 opcode) /* TST */
{
  {
    {
      uae_s8 src = get_ibyte(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(src)) == 0);
      SET_NFLG(((uae_s8)(src)) < 0);
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4a40_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(src)) == 0);
      SET_NFLG(((uae_s16)(src)) < 0);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4a48_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(src)) == 0);
      SET_NFLG(((uae_s16)(src)) < 0);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4a50_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4a58_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4a60_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_4a68_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a70_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a78_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a79_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_4a7a_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a7b_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_4a7c_3(uae_u32 opcode) /* TST */
{
  {
    {
      uae_s16 src = get_iword(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s16)(src)) == 0);
      SET_NFLG(((uae_s16)(src)) < 0);
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4a80_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(src)) == 0);
      SET_NFLG(((uae_s32)(src)) < 0);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4a88_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(src)) == 0);
      SET_NFLG(((uae_s32)(src)) < 0);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4a90_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4a98_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4aa0_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4aa8_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4ab0_3(uae_u32 opcode) /* TST */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4ab8_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4ab9_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4aba_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4abb_3(uae_u32 opcode) /* TST */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4abc_3(uae_u32 opcode) /* TST */
{
  {
    {
      uae_s32 src = get_ilong(2);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s32)(src)) == 0);
      SET_NFLG(((uae_s32)(src)) < 0);
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4ac0_3(uae_u32 opcode) /* TAS */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      CLEAR_CZNV;
      SET_ZFLG(((uae_s8)(src)) == 0);
      SET_NFLG(((uae_s8)(src)) < 0);
      src |= 0x80;
      m68k_dreg(regs, srcreg) =
          (m68k_dreg(regs, srcreg) & ~0xff) | ((src) & 0xff);
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4ad0_3(uae_u32 opcode) /* TAS */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        src |= 0x80;
        put_byte(srca, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4ad8_3(uae_u32 opcode) /* TAS */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        src |= 0x80;
        put_byte(srca, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4ae0_3(uae_u32 opcode) /* TAS */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        src |= 0x80;
        put_byte(srca, src);
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4ae8_3(uae_u32 opcode) /* TAS */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        src |= 0x80;
        put_byte(srca, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4af0_3(uae_u32 opcode) /* TAS */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        src |= 0x80;
        put_byte(srca, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4af8_3(uae_u32 opcode) /* TAS */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        src |= 0x80;
        put_byte(srca, src);
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4af9_3(uae_u32 opcode) /* TAS */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        src |= 0x80;
        put_byte(srca, src);
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_4c90_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_areg(regs, dstreg);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4c98_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_areg(regs, dstreg);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
        m68k_areg(regs, dstreg) = srca;
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4ca8_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cb0_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cb8_3(uae_u32 opcode) /* MVMEL */
{
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cb9_3(uae_u32 opcode) /* MVMEL */
{
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = get_ilong(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(8);
  return 8;
}
unsigned long REGPARAM2 op_4cba_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = 2;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_getpc() + 4;
      srca += (uae_s32)(uae_s16)get_iword(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cbb_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = 3;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr tmppc = m68k_getpc() + 4;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(4));
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) =
              (uae_s32)(uae_s16)get_word(srca);
          srca += 2;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cd0_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_areg(regs, dstreg);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4cd8_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_areg(regs, dstreg);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
        m68k_areg(regs, dstreg) = srca;
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_4ce8_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cf0_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = opcode & 7;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(4));
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cf8_3(uae_u32 opcode) /* MVMEL */
{
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cf9_3(uae_u32 opcode) /* MVMEL */
{
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = get_ilong(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(8);
  return 8;
}
unsigned long REGPARAM2 op_4cfa_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = 2;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr srca = m68k_getpc() + 4;
      srca += (uae_s32)(uae_s16)get_iword(4);
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4cfb_3(uae_u32 opcode) /* MVMEL */
{
  uae_u32 dstreg = 3;
  {
    uae_u16 mask = get_iword(2);
    unsigned int dmask = mask & 0xff, amask = (mask >> 8) & 0xff;
    {
      uaecptr tmppc = m68k_getpc() + 4;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(4));
      {
        while (dmask) {
          m68k_dreg(regs, movem_index1[dmask]) = get_long(srca);
          srca += 4;
          dmask = movem_next[dmask];
        }
        while (amask) {
          m68k_areg(regs, movem_index1[amask]) = get_long(srca);
          srca += 4;
          amask = movem_next[amask];
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_4e40_3(uae_u32 opcode) /* TRAP */
{
  uae_u32 srcreg = (opcode & 15);
  {
    {
      uae_u32 src = srcreg;
      m68k_incpc(2);
      Exception(src + 32, 0);
    }
  }
  return 2;
}
unsigned long REGPARAM2 op_4e50_3(uae_u32 opcode) /* LINK */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr olda = m68k_areg(regs, 7) - 4;
      m68k_areg(regs, 7) = olda;
      if (7 == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        uae_s32 src = m68k_areg(regs, srcreg);
        put_long(olda, src);
        m68k_areg(regs, srcreg) = (m68k_areg(regs, 7));
        if (srcreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
        {
          uae_s16 offs = get_iword(2);
          m68k_areg(regs, 7) += offs;
          CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_4e58_3(uae_u32 opcode) /* UNLK */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      m68k_areg(regs, 7) = src;
      CHECK_STACK_POINTER_INCREMENT();
      {
        uaecptr olda = m68k_areg(regs, 7);
        {
          uae_s32 old = get_long(olda);
          m68k_areg(regs, 7) += 4;
          m68k_areg(regs, srcreg) = (old);
          if (srcreg == 7)
            CHECK_STACK_POINTER_ASSIGNMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_4e71_3(uae_u32 opcode) /* NOP */
{
  {}
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_4e74_3(uae_u32 opcode) /* RTD */
{
  {
    compiler_flush_jsr_stack();
    {
      uaecptr pca = m68k_areg(regs, 7);
      {
        uae_s32 pc = get_long(pca);
        m68k_areg(regs, 7) += 4;
        {
          uae_s16 offs = get_iword(2);
          m68k_areg(regs, 7) += offs;
          m68k_setpc_rte(pc);
        }
      }
    }
  }
  return 8;
}
unsigned long REGPARAM2 op_4e75_3(uae_u32 opcode) /* RTS */
{
  { m68k_do_rts(); }
  return 2;
}
unsigned long REGPARAM2 op_4e76_3(uae_u32 opcode) /* TRAPV */
{
  {
    m68k_incpc(2);
    if (GET_VFLG) {
      Exception(7, m68k_getpc());
      goto endlabel803;
    }
  }
endlabel803:;
  return 2;
}
unsigned long REGPARAM2 op_4e77_3(uae_u32 opcode) /* RTR */
{
  {
    compiler_flush_jsr_stack();
    MakeSR();
    {
      uaecptr sra = m68k_areg(regs, 7);
      {
        uae_s16 sr = get_word(sra);
        m68k_areg(regs, 7) += 2;
        {
          uaecptr pca = m68k_areg(regs, 7);
          {
            uae_s32 pc = get_long(pca);
            m68k_areg(regs, 7) += 4;
            regs.sr &= 0xFF00;
            sr &= 0xFF;
            regs.sr |= sr;
            m68k_setpc(pc);
            MakeFromSR();
          }
        }
      }
    }
  }
  return 8;
}
unsigned long REGPARAM2 op_4e90_3(uae_u32 opcode) /* JSR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_do_jsr(m68k_getpc() + 2, srca);
    }
  }
  return 2;
}
unsigned long REGPARAM2 op_4ea8_3(uae_u32 opcode) /* JSR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      m68k_do_jsr(m68k_getpc() + 4, srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4eb0_3(uae_u32 opcode) /* JSR */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      m68k_do_jsr(m68k_getpc() + 4, srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4eb8_3(uae_u32 opcode) /* JSR */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      m68k_do_jsr(m68k_getpc() + 4, srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4eb9_3(uae_u32 opcode) /* JSR */
{
  {
    {
      uaecptr srca = get_ilong(2);
      m68k_do_jsr(m68k_getpc() + 6, srca);
    }
  }
  return 6;
}
unsigned long REGPARAM2 op_4eba_3(uae_u32 opcode) /* JSR */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      m68k_do_jsr(m68k_getpc() + 4, srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4ebb_3(uae_u32 opcode) /* JSR */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      m68k_do_jsr(m68k_getpc() + 4, srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4ed0_3(uae_u32 opcode) /* JMP */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_setpc(srca);
    }
  }
  return 2;
}
unsigned long REGPARAM2 op_4ee8_3(uae_u32 opcode) /* JMP */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      m68k_setpc(srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4ef0_3(uae_u32 opcode) /* JMP */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      m68k_setpc(srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4ef8_3(uae_u32 opcode) /* JMP */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      m68k_setpc(srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4ef9_3(uae_u32 opcode) /* JMP */
{
  {
    {
      uaecptr srca = get_ilong(2);
      m68k_setpc(srca);
    }
  }
  return 6;
}
unsigned long REGPARAM2 op_4efa_3(uae_u32 opcode) /* JMP */
{
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      m68k_setpc(srca);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_4efb_3(uae_u32 opcode) /* JMP */
{
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      m68k_setpc(srca);
    }
  }
  return 4;
}
#endif

#ifdef PART_5
unsigned long REGPARAM2 op_5000_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5010_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5018_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5020_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5028_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5030_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5038_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5039_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_5040_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5048_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5050_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5058_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5060_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5068_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5070_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5078_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5079_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_5080_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5088_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5090_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_5098_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_50a0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_50a8_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_50b0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_50b8_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_50b9_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_50c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(0) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_50c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(0)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel846;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel846:;
  return 4;
}
unsigned long REGPARAM2 op_50d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(0) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_50d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(0) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_50e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(0) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_50e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(0) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_50f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(0) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_50f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(0) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_50f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(0) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_5100_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5110_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5118_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5120_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5128_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5130_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5138_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5139_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_5140_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5148_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5150_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5158_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5160_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_5168_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5170_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5178_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_5179_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_5180_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5188_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5190_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_5198_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_51a0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_51a8_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_51b0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_51b8_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_51b9_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  {
    {
      uae_u32 src = srcreg;
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_51c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(1) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_51c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(1)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel881;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel881:;
  return 4;
}
unsigned long REGPARAM2 op_51d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(1) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_51d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(1) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_51e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(1) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_51e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(1) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_51f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(1) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_51f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(1) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_51f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(1) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_52c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(2) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_52c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(2)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel890;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel890:;
  return 4;
}
unsigned long REGPARAM2 op_52d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(2) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_52d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(2) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_52e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(2) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_52e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(2) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_52f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(2) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_52f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(2) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_52f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(2) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_53c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(3) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_53c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(3)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel899;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel899:;
  return 4;
}
unsigned long REGPARAM2 op_53d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(3) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_53d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(3) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_53e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(3) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_53e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(3) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_53f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(3) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_53f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(3) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_53f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(3) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_54c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(4) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_54c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(4)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel908;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel908:;
  return 4;
}
unsigned long REGPARAM2 op_54d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(4) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_54d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(4) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_54e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(4) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_54e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(4) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_54f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(4) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_54f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(4) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_54f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(4) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_55c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(5) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_55c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(5)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel917;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel917:;
  return 4;
}
unsigned long REGPARAM2 op_55d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(5) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_55d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(5) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_55e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(5) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_55e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(5) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_55f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(5) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_55f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(5) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_55f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(5) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_56c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(6) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_56c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(6)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel926;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel926:;
  return 4;
}
unsigned long REGPARAM2 op_56d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(6) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_56d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(6) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_56e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(6) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_56e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(6) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_56f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(6) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_56f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(6) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_56f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(6) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_57c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(7) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_57c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(7)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel935;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel935:;
  return 4;
}
unsigned long REGPARAM2 op_57d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(7) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_57d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(7) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_57e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(7) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_57e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(7) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_57f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(7) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_57f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(7) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_57f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(7) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_58c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(8) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_58c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(8)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel944;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel944:;
  return 4;
}
unsigned long REGPARAM2 op_58d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(8) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_58d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(8) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_58e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(8) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_58e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(8) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_58f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(8) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_58f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(8) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_58f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(8) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_59c0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(9) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_59c8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(9)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel953;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel953:;
  return 4;
}
unsigned long REGPARAM2 op_59d0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(9) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_59d8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(9) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_59e0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(9) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_59e8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(9) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_59f0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(9) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_59f8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(9) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_59f9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(9) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_5ac0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(10) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5ac8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(10)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel962;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel962:;
  return 4;
}
unsigned long REGPARAM2 op_5ad0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(10) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ad8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(10) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ae0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(10) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ae8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(10) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5af0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(10) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5af8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(10) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5af9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(10) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_5bc0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(11) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5bc8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(11)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel971;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel971:;
  return 4;
}
unsigned long REGPARAM2 op_5bd0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(11) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5bd8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(11) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5be0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(11) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5be8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(11) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5bf0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(11) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5bf8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(11) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5bf9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(11) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_5cc0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(12) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5cc8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(12)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel980;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel980:;
  return 4;
}
unsigned long REGPARAM2 op_5cd0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(12) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5cd8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(12) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ce0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(12) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ce8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(12) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5cf0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(12) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5cf8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(12) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5cf9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(12) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_5dc0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(13) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5dc8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(13)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel989;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel989:;
  return 4;
}
unsigned long REGPARAM2 op_5dd0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(13) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5dd8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(13) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5de0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(13) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5de8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(13) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5df0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(13) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5df8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(13) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5df9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(13) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_5ec0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(14) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5ec8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(14)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel998;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel998:;
  return 4;
}
unsigned long REGPARAM2 op_5ed0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(14) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ed8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(14) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ee0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(14) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5ee8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(14) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5ef0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(14) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5ef8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(14) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5ef9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(14) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
#endif

#ifdef PART_6
unsigned long REGPARAM2 op_5fc0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      {
        int val = cctrue(15) ? 0xff : 0;
#if HAS_PROFILING
        if (gProfilingEnabled)
          ProfileIncrementClock(2);
#endif
        m68k_dreg(regs, srcreg) =
            (m68k_dreg(regs, srcreg) & ~0xff) | ((val) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_5fc8_3(uae_u32 opcode) /* DBcc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 offs = get_iword(2);
        if (!cctrue(15)) {
          m68k_dreg(regs, srcreg) =
              (m68k_dreg(regs, srcreg) & ~0xffff) | (((src - 1)) & 0xffff);
          if (src) {
            m68k_incpc((uae_s32)offs + 2);
#if HAS_PROFILING
            if (gProfilingEnabled)
              ProfileIncrementClock(2);
#endif
            goto endlabel1007;
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1007:;
  return 4;
}
unsigned long REGPARAM2 op_5fd0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        int val = cctrue(15) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5fd8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
      {
        int val = cctrue(15) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5fe0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      m68k_areg(regs, srcreg) = srca;
      if (srcreg == 7)
        CHECK_STACK_POINTER_DECREMENT();
      {
        int val = cctrue(15) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_5fe8_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(15) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5ff0_3(uae_u32 opcode) /* Scc */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        int val = cctrue(15) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5ff8_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        int val = cctrue(15) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_5ff9_3(uae_u32 opcode) /* Scc */
{
  {
    {
      uaecptr srca = get_ilong(2);
      {
        int val = cctrue(15) ? 0xff : 0;
        put_byte(srca, val);
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_6000_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(0))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1015;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1015:;
  return 4;
}
unsigned long REGPARAM2 op_6001_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(0))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1016;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1016:;
  return 2;
}
unsigned long REGPARAM2 op_6100_3(uae_u32 opcode) /* BSR */
{
  {
    {
      uae_s16 src = get_iword(2);
      uae_s32 s = (uae_s32)src + 2;
      m68k_do_bsr(m68k_getpc() + 4, s);
    }
  }
  return 4;
}
unsigned long REGPARAM2 op_6101_3(uae_u32 opcode) /* BSR */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      uae_s32 s = (uae_s32)src + 2;
      m68k_do_bsr(m68k_getpc() + 2, s);
    }
  }
  return 2;
}
unsigned long REGPARAM2 op_6200_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(2))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1021;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1021:;
  return 4;
}
unsigned long REGPARAM2 op_6201_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(2))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1022;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1022:;
  return 2;
}
unsigned long REGPARAM2 op_6300_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(3))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1024;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1024:;
  return 4;
}
unsigned long REGPARAM2 op_6301_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(3))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1025;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1025:;
  return 2;
}
unsigned long REGPARAM2 op_6400_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(4))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1027;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1027:;
  return 4;
}
unsigned long REGPARAM2 op_6401_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(4))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1028;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1028:;
  return 2;
}
unsigned long REGPARAM2 op_6500_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(5))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1030;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1030:;
  return 4;
}
unsigned long REGPARAM2 op_6501_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(5))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1031;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1031:;
  return 2;
}
unsigned long REGPARAM2 op_6600_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(6))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1033;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1033:;
  return 4;
}
unsigned long REGPARAM2 op_6601_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(6))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1034;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1034:;
  return 2;
}
unsigned long REGPARAM2 op_6700_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(7))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1036;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1036:;
  return 4;
}
unsigned long REGPARAM2 op_6701_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(7))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1037;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1037:;
  return 2;
}
unsigned long REGPARAM2 op_6800_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(8))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1039;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1039:;
  return 4;
}
unsigned long REGPARAM2 op_6801_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(8))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1040;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1040:;
  return 2;
}
unsigned long REGPARAM2 op_6900_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(9))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1042;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1042:;
  return 4;
}
unsigned long REGPARAM2 op_6901_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(9))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1043;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1043:;
  return 2;
}
unsigned long REGPARAM2 op_6a00_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(10))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1045;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1045:;
  return 4;
}
unsigned long REGPARAM2 op_6a01_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(10))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1046;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1046:;
  return 2;
}
unsigned long REGPARAM2 op_6b00_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(11))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1048;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1048:;
  return 4;
}
unsigned long REGPARAM2 op_6b01_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(11))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1049;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1049:;
  return 2;
}
unsigned long REGPARAM2 op_6c00_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(12))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1051;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1051:;
  return 4;
}
unsigned long REGPARAM2 op_6c01_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(12))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1052;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1052:;
  return 2;
}
unsigned long REGPARAM2 op_6d00_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(13))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1054;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1054:;
  return 4;
}
unsigned long REGPARAM2 op_6d01_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(13))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1055;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1055:;
  return 2;
}
unsigned long REGPARAM2 op_6e00_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(14))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1057;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1057:;
  return 4;
}
unsigned long REGPARAM2 op_6e01_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(14))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1058;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1058:;
  return 2;
}
unsigned long REGPARAM2 op_6f00_3(uae_u32 opcode) /* Bcc */
{
  {
    {
      uae_s16 src = get_iword(2);
      if (!cctrue(15))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1060;
    didnt_jump:;
    }
  }
  m68k_incpc(4);
endlabel1060:;
  return 4;
}
unsigned long REGPARAM2 op_6f01_3(uae_u32 opcode) /* Bcc */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  {
    {
      uae_u32 src = srcreg;
      if (!cctrue(15))
        goto didnt_jump;
      m68k_incpc((uae_s32)src + 2);
#if HAS_PROFILING
      if (gProfilingEnabled)
        ProfileIncrementClock(2);
#endif
      goto endlabel1061;
    didnt_jump:;
    }
  }
  m68k_incpc(2);
endlabel1061:;
  return 2;
}
unsigned long REGPARAM2 op_7000_3(uae_u32 opcode) /* MOVE */
{
  uae_u32 srcreg = (uae_s32)(uae_s8)(opcode & 255);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_u32 src = srcreg;
      {
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_8000_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_8010_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_8018_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_8020_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_8028_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_8030_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_8038_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_8039_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_803a_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_803b_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_803c_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_8040_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_8050_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_8058_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_8060_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_8068_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_8070_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_8078_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_8079_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_807a_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_807b_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_807c_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_8080_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_8090_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_8098_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_80a0_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_80a8_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_80b0_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_80b8_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_80b9_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_80ba_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_80bb_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_80bc_3(uae_u32 opcode) /* OR */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src |= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_80c0_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        if (src == 0) {
          Exception(5, oldpc);
          goto endlabel1097;
        } else {
          uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
          uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
          if (newv > 0xffff) {
            SET_VFLG(1);
            SET_NFLG(1);
            SET_CFLG(0);
          } else {
            CLEAR_CZNV;
            SET_ZFLG(((uae_s16)(newv)) == 0);
            SET_NFLG(((uae_s16)(newv)) < 0);
            newv = (newv & 0xffff) | ((uae_u32)rem << 16);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1097:;
  return 70;
}
unsigned long REGPARAM2 op_80d0_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1098;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1098:;
  return 72;
}
unsigned long REGPARAM2 op_80d8_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1099;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1099:;
  return 72;
}
unsigned long REGPARAM2 op_80e0_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1100;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1100:;
  return 72;
}
unsigned long REGPARAM2 op_80e8_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1101;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1101:;
  return 74;
}
unsigned long REGPARAM2 op_80f0_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1102;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1102:;
  return 74;
}
unsigned long REGPARAM2 op_80f8_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1103;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1103:;
  return 74;
}
unsigned long REGPARAM2 op_80f9_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1104;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
endlabel1104:;
  return 76;
}
unsigned long REGPARAM2 op_80fa_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1105;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1105:;
  return 74;
}
unsigned long REGPARAM2 op_80fb_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1106;
          } else {
            uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
            uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
            if (newv > 0xffff) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1106:;
  return 74;
}
unsigned long REGPARAM2 op_80fc_3(uae_u32 opcode) /* DIVU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        if (src == 0) {
          Exception(5, oldpc);
          goto endlabel1107;
        } else {
          uae_u32 newv = (uae_u32)dst / (uae_u32)(uae_u16)src;
          uae_u32 rem = (uae_u32)dst % (uae_u32)(uae_u16)src;
          if (newv > 0xffff) {
            SET_VFLG(1);
            SET_NFLG(1);
            SET_CFLG(0);
          } else {
            CLEAR_CZNV;
            SET_ZFLG(((uae_s16)(newv)) == 0);
            SET_NFLG(((uae_s16)(newv)) < 0);
            newv = (newv & 0xffff) | ((uae_u32)rem << 16);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1107:;
  return 72;
}
unsigned long REGPARAM2 op_8100_3(uae_u32 opcode) /* SBCD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          uae_u16 newv_lo = (dst & 0xF) - (src & 0xF) - (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = (dst & 0xF0) - (src & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo -= 6;
            newv_hi -= 0x10;
          }
          newv = newv_hi + (newv_lo & 0xF);
          SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv -= 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(dst)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
            m68k_dreg(regs, dstreg) =
                (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_8108_3(uae_u32 opcode) /* SBCD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          {
            uae_s8 dst = get_byte(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u16 newv_lo = (dst & 0xF) - (src & 0xF) - (GET_XFLG ? 1 : 0);
              uae_u16 newv_hi = (dst & 0xF0) - (src & 0xF0);
              uae_u16 newv;
              int cflg;
              if (newv_lo > 9) {
                newv_lo -= 6;
                newv_hi -= 0x10;
              }
              newv = newv_hi + (newv_lo & 0xF);
              SET_CFLG(cflg = (newv_hi & 0x1F0) > 0x90);
              COPY_CARRY;
              if (cflg)
                newv -= 0x60;
              SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
              SET_NFLG(((uae_s8)(newv)) < 0);
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 8;
}
unsigned long REGPARAM2 op_8110_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_8118_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_8120_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_8128_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8130_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8138_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8139_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8150_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_8158_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_8160_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_8168_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8170_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8178_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_8179_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s16 dst = get_word(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_8190_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_8198_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_81a0_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_81a8_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_81b0_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_81b8_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_81b9_3(uae_u32 opcode) /* OR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s32 dst = get_long(dsta);
          src |= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_81c0_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        if (src == 0) {
          Exception(5, oldpc);
          goto endlabel1131;
        } else {
          uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
          uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
          if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
            SET_VFLG(1);
            SET_NFLG(1);
            SET_CFLG(0);
          } else {
            if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
              rem = -rem;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s16)(newv)) == 0);
            SET_NFLG(((uae_s16)(newv)) < 0);
            newv = (newv & 0xffff) | ((uae_u32)rem << 16);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1131:;
  return 74;
}
unsigned long REGPARAM2 op_81d0_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1132;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1132:;
  return 76;
}
unsigned long REGPARAM2 op_81d8_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1133;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1133:;
  return 76;
}
unsigned long REGPARAM2 op_81e0_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1134;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
endlabel1134:;
  return 76;
}
unsigned long REGPARAM2 op_81e8_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1135;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1135:;
  return 78;
}
unsigned long REGPARAM2 op_81f0_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1136;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1136:;
  return 78;
}
unsigned long REGPARAM2 op_81f8_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1137;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1137:;
  return 78;
}
unsigned long REGPARAM2 op_81f9_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1138;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
endlabel1138:;
  return 80;
}
unsigned long REGPARAM2 op_81fa_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1139;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1139:;
  return 78;
}
unsigned long REGPARAM2 op_81fb_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          if (src == 0) {
            Exception(5, oldpc);
            goto endlabel1140;
          } else {
            uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
            uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
            if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
              SET_VFLG(1);
              SET_NFLG(1);
              SET_CFLG(0);
            } else {
              if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
                rem = -rem;
              CLEAR_CZNV;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_NFLG(((uae_s16)(newv)) < 0);
              newv = (newv & 0xffff) | ((uae_u32)rem << 16);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1140:;
  return 78;
}
unsigned long REGPARAM2 op_81fc_3(uae_u32 opcode) /* DIVS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    uaecptr oldpc = m68k_getpc();
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        if (src == 0) {
          Exception(5, oldpc);
          goto endlabel1141;
        } else {
          uae_s32 newv = (uae_s32)dst / (uae_s32)(uae_s16)src;
          uae_u16 rem = (uae_s32)dst % (uae_s32)(uae_s16)src;
          if ((newv & 0xffff8000) != 0 && (newv & 0xffff8000) != 0xffff8000) {
            SET_VFLG(1);
            SET_NFLG(1);
            SET_CFLG(0);
          } else {
            if (((uae_s16)rem < 0) != ((uae_s32)dst < 0))
              rem = -rem;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s16)(newv)) == 0);
            SET_NFLG(((uae_s16)(newv)) < 0);
            newv = (newv & 0xffff) | ((uae_u32)rem << 16);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
endlabel1141:;
  return 76;
}
unsigned long REGPARAM2 op_9000_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9010_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_9018_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_9020_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_9028_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_9030_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_9038_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_9039_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_903a_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_903b_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_903c_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_9040_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9048_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9050_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_9058_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_9060_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_9068_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_9070_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_9078_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_9079_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_907a_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_907b_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_907c_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_9080_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9088_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9090_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_9098_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_90a0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_90a8_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_90b0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_90b8_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_90b9_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_90ba_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_90bb_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_90bc_3(uae_u32 opcode) /* SUB */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_90c0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_90c8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_90d0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_90d8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_90e0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_90e8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_90f0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_90f8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_90f9_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_90fa_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_90fb_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_90fc_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_9100_3(uae_u32 opcode) /* SUBX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(dst)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            m68k_dreg(regs, dstreg) =
                (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9108_3(uae_u32 opcode) /* SUBX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          {
            uae_s8 dst = get_byte(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
                SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
                COPY_CARRY;
                SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
                SET_NFLG(((uae_s8)(newv)) < 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 8;
}
unsigned long REGPARAM2 op_9110_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_9118_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_9120_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_9128_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_9130_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_9138_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_9139_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_9140_3(uae_u32 opcode) /* SUBX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(dst)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            m68k_dreg(regs, dstreg) =
                (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9148_3(uae_u32 opcode) /* SUBX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          {
            uae_s16 dst = get_word(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
                SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
                COPY_CARRY;
                SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
                SET_NFLG(((uae_s16)(newv)) < 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 8;
}
unsigned long REGPARAM2 op_9150_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_9158_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_9160_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_9168_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_9170_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_9178_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_9179_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_9180_3(uae_u32 opcode) /* SUBX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(dst)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_9188_3(uae_u32 opcode) /* SUBX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          {
            uae_s32 dst = get_long(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u32 newv = dst - src - (GET_XFLG ? 1 : 0);
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_VFLG((flgs ^ flgo) & (flgo ^ flgn));
                SET_CFLG(flgs ^ ((flgs ^ flgn) & (flgo ^ flgn)));
                COPY_CARRY;
                SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
                SET_NFLG(((uae_s32)(newv)) < 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 14;
}
unsigned long REGPARAM2 op_9190_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_9198_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_91a0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_91a8_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_91b0_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_91b8_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_91b9_3(uae_u32 opcode) /* SUB */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_91c0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_91c8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_91d0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_91d8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_91e0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_91e8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_91f0_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_91f8_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_91f9_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_91fa_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_91fb_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst - src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_91fc_3(uae_u32 opcode) /* SUBA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst - src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_b000_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b010_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
#endif

#ifdef PART_7
unsigned long REGPARAM2 op_b018_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b020_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b028_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b030_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b038_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b039_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_b03a_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b03b_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b03c_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_b040_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b048_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b050_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b058_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b060_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b068_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b070_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b078_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b079_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_b07a_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b07b_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b07c_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_b080_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b088_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b090_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b098_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b0a0_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b0a8_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b0b0_3(uae_u32 opcode) /* CMP */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b0b8_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b0b9_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_b0ba_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b0bb_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b0bc_3(uae_u32 opcode) /* CMP */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_b0c0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b0c8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b0d0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b0d8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b0e0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_b0e8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b0f0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b0f8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b0f9_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_b0fa_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b0fb_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_b0fc_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_b100_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src ^= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b108_3(uae_u32 opcode) /* CMPM */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          {
            uae_s8 dst = get_byte(dsta);
            m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
            {
              {
                uae_u32 newv = ((uae_s8)(dst)) - ((uae_s8)(src));
                {
                  int flgs = ((uae_s8)(src)) < 0;
                  int flgo = ((uae_s8)(dst)) < 0;
                  int flgn = ((uae_s8)(newv)) < 0;
                  SET_ZFLG(((uae_s8)(newv)) == 0);
                  SET_VFLG((flgs != flgo) && (flgn != flgo));
                  SET_CFLG(((uae_u8)(src)) > ((uae_u8)(dst)));
                  SET_NFLG(flgn != 0);
                }
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b110_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b118_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b120_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b128_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b130_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b138_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b139_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_b140_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src ^= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b148_3(uae_u32 opcode) /* CMPM */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          {
            uae_s16 dst = get_word(dsta);
            m68k_areg(regs, dstreg) += 2;
            {
              {
                uae_u32 newv = ((uae_s16)(dst)) - ((uae_s16)(src));
                {
                  int flgs = ((uae_s16)(src)) < 0;
                  int flgo = ((uae_s16)(dst)) < 0;
                  int flgn = ((uae_s16)(newv)) < 0;
                  SET_ZFLG(((uae_s16)(newv)) == 0);
                  SET_VFLG((flgs != flgo) && (flgn != flgo));
                  SET_CFLG(((uae_u16)(src)) > ((uae_u16)(dst)));
                  SET_NFLG(flgn != 0);
                }
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b150_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b158_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b160_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b168_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b170_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b178_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b179_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s16 dst = get_word(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_b180_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src ^= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b188_3(uae_u32 opcode) /* CMPM */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uaecptr dsta = m68k_areg(regs, dstreg);
          {
            uae_s32 dst = get_long(dsta);
            m68k_areg(regs, dstreg) += 4;
            {
              {
                uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
                {
                  int flgs = ((uae_s32)(src)) < 0;
                  int flgo = ((uae_s32)(dst)) < 0;
                  int flgn = ((uae_s32)(newv)) < 0;
                  SET_ZFLG(((uae_s32)(newv)) == 0);
                  SET_VFLG((flgs != flgo) && (flgn != flgo));
                  SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                  SET_NFLG(flgn != 0);
                }
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_b190_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_b198_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_b1a0_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_b1a8_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_b1b0_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_b1b8_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_b1b9_3(uae_u32 opcode) /* EOR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s32 dst = get_long(dsta);
          src ^= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_b1c0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b1c8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_b1d0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b1d8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b1e0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_b1e8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b1f0_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b1f8_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b1f9_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_b1fa_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b1fb_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs != flgo) && (flgn != flgo));
                SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
                SET_NFLG(flgn != 0);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_b1fc_3(uae_u32 opcode) /* CMPA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) - ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs != flgo) && (flgn != flgo));
              SET_CFLG(((uae_u32)(src)) > ((uae_u32)(dst)));
              SET_NFLG(flgn != 0);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_c000_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_c010_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_c018_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_c020_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_c028_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c030_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c038_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c039_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c03a_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c03b_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c03c_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s8)(src)) == 0);
        SET_NFLG(((uae_s8)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xff) | ((src) & 0xff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_c040_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_c050_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_c058_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_c060_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_c068_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c070_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c078_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c079_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_c07a_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c07b_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_c07c_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s16)(src)) == 0);
        SET_NFLG(((uae_s16)(src)) < 0);
        m68k_dreg(regs, dstreg) =
            (m68k_dreg(regs, dstreg) & ~0xffff) | ((src) & 0xffff);
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_c080_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_c090_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c098_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c0a0_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c0a8_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c0b0_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c0b8_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c0b9_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_c0ba_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c0bb_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          m68k_dreg(regs, dstreg) = (src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c0bc_3(uae_u32 opcode) /* AND */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        src &= dst;
        CLEAR_CZNV;
        SET_ZFLG(((uae_s32)(src)) == 0);
        SET_NFLG(((uae_s32)(src)) < 0);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_c0c0_3(uae_u32 opcode) /* MULU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(newv)) == 0);
          SET_NFLG(((uae_s32)(newv)) < 0);
          m68k_dreg(regs, dstreg) = (newv);
        }
      }
    }
  }
  m68k_incpc(2);
  return 34;
}
unsigned long REGPARAM2 op_c0d0_3(uae_u32 opcode) /* MULU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 36;
}
unsigned long REGPARAM2 op_c0d8_3(uae_u32 opcode) /* MULU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 36;
}
unsigned long REGPARAM2 op_c0e0_3(uae_u32 opcode) /* MULU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 36;
}
unsigned long REGPARAM2 op_c0e8_3(uae_u32 opcode) /* MULU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c0f0_3(uae_u32 opcode) /* MULU */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c0f8_3(uae_u32 opcode) /* MULU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c0f9_3(uae_u32 opcode) /* MULU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 40;
}
unsigned long REGPARAM2 op_c0fa_3(uae_u32 opcode) /* MULU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c0fb_3(uae_u32 opcode) /* MULU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c0fc_3(uae_u32 opcode) /* MULU */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = (uae_u32)(uae_u16)dst * (uae_u32)(uae_u16)src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(newv)) == 0);
          SET_NFLG(((uae_s32)(newv)) < 0);
          m68k_dreg(regs, dstreg) = (newv);
        }
      }
    }
  }
  m68k_incpc(4);
  return 36;
}
unsigned long REGPARAM2 op_c100_3(uae_u32 opcode) /* ABCD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          uae_u16 newv_lo = (src & 0xF) + (dst & 0xF) + (GET_XFLG ? 1 : 0);
          uae_u16 newv_hi = (src & 0xF0) + (dst & 0xF0);
          uae_u16 newv;
          int cflg;
          if (newv_lo > 9) {
            newv_lo += 6;
          }
          newv = newv_hi + newv_lo;
          SET_CFLG(cflg = (newv & 0x1F0) > 0x90);
          COPY_CARRY;
          if (cflg)
            newv += 0x60;
          SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
          SET_NFLG(((uae_s8)(newv)) < 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(dst)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
            m68k_dreg(regs, dstreg) =
                (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_c108_3(uae_u32 opcode) /* ABCD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          {
            uae_s8 dst = get_byte(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u16 newv_lo = (src & 0xF) + (dst & 0xF) + (GET_XFLG ? 1 : 0);
              uae_u16 newv_hi = (src & 0xF0) + (dst & 0xF0);
              uae_u16 newv;
              int cflg;
              if (newv_lo > 9) {
                newv_lo += 6;
              }
              newv = newv_hi + newv_lo;
              SET_CFLG(cflg = (newv & 0x1F0) > 0x90);
              COPY_CARRY;
              if (cflg)
                newv += 0x60;
              SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
              SET_NFLG(((uae_s8)(newv)) < 0);
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_VFLG((flgs ^ flgo) & (flgn ^ flgo));
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 8;
}
unsigned long REGPARAM2 op_c110_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c118_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c120_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c128_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c130_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c138_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c139_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s8)(src)) == 0);
          SET_NFLG(((uae_s8)(src)) < 0);
          put_byte(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_c140_3(uae_u32 opcode) /* EXG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        m68k_dreg(regs, srcreg) = (dst);
        m68k_dreg(regs, dstreg) = (src);
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_c148_3(uae_u32 opcode) /* EXG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        m68k_areg(regs, srcreg) = (dst);
        if (srcreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
        m68k_areg(regs, dstreg) = (src);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_c150_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c158_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c160_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_c168_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c170_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c178_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_c179_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s16 dst = get_word(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(src)) == 0);
          SET_NFLG(((uae_s16)(src)) < 0);
          put_word(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_c188_3(uae_u32 opcode) /* EXG */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        m68k_dreg(regs, srcreg) = (dst);
        m68k_areg(regs, dstreg) = (src);
        if (dstreg == 7)
          CHECK_STACK_POINTER_ASSIGNMENT();
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_c190_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_c198_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_c1a0_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_c1a8_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_c1b0_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_c1b8_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_c1b9_3(uae_u32 opcode) /* AND */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s32 dst = get_long(dsta);
          src &= dst;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(src)) == 0);
          SET_NFLG(((uae_s32)(src)) < 0);
          put_long(dsta, src);
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_c1c0_3(uae_u32 opcode) /* MULS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(newv)) == 0);
          SET_NFLG(((uae_s32)(newv)) < 0);
          m68k_dreg(regs, dstreg) = (newv);
        }
      }
    }
  }
  m68k_incpc(2);
  return 34;
}
unsigned long REGPARAM2 op_c1d0_3(uae_u32 opcode) /* MULS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 36;
}
unsigned long REGPARAM2 op_c1d8_3(uae_u32 opcode) /* MULS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 36;
}
unsigned long REGPARAM2 op_c1e0_3(uae_u32 opcode) /* MULS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 36;
}
unsigned long REGPARAM2 op_c1e8_3(uae_u32 opcode) /* MULS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c1f0_3(uae_u32 opcode) /* MULS */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c1f8_3(uae_u32 opcode) /* MULS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c1f9_3(uae_u32 opcode) /* MULS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 40;
}
unsigned long REGPARAM2 op_c1fa_3(uae_u32 opcode) /* MULS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c1fb_3(uae_u32 opcode) /* MULS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
            CLEAR_CZNV;
            SET_ZFLG(((uae_s32)(newv)) == 0);
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 38;
}
unsigned long REGPARAM2 op_c1fc_3(uae_u32 opcode) /* MULS */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = (uae_s32)(uae_s16)dst * (uae_s32)(uae_s16)src;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s32)(newv)) == 0);
          SET_NFLG(((uae_s32)(newv)) < 0);
          m68k_dreg(regs, dstreg) = (newv);
        }
      }
    }
  }
  m68k_incpc(4);
  return 36;
}
unsigned long REGPARAM2 op_d000_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d010_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d018_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) += areg_byteinc[srcreg];
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d020_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d028_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d030_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d038_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d039_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_d03a_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d03b_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s8 src = get_byte(srca);
        {
          uae_s8 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d03c_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = get_ibyte(2);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
            {
              int flgs = ((uae_s8)(src)) < 0;
              int flgo = ((uae_s8)(dst)) < 0;
              int flgn = ((uae_s8)(newv)) < 0;
              SET_ZFLG(((uae_s8)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_d040_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d048_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d050_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d058_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d060_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d068_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d070_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d078_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d079_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_d07a_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d07b_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s16 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) =
                    (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d07c_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
            {
              int flgs = ((uae_s16)(src)) < 0;
              int flgo = ((uae_s16)(dst)) < 0;
              int flgn = ((uae_s16)(newv)) < 0;
              SET_ZFLG(((uae_s16)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) =
                  (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_d080_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d088_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d090_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d098_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d0a0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d0a8_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d0b0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d0b8_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d0b9_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_d0ba_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d0bb_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_dreg(regs, dstreg);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                m68k_dreg(regs, dstreg) = (newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d0bc_3(uae_u32 opcode) /* ADD */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          {
            uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
            {
              int flgs = ((uae_s32)(src)) < 0;
              int flgo = ((uae_s32)(dst)) < 0;
              int flgn = ((uae_s32)(newv)) < 0;
              SET_ZFLG(((uae_s32)(newv)) == 0);
              SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
              SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
              COPY_CARRY;
              SET_NFLG(flgn != 0);
              m68k_dreg(regs, dstreg) = (newv);
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_d0c0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d0c8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d0d0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d0d8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d0e0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 4;
}
unsigned long REGPARAM2 op_d0e8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d0f0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d0f8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d0f9_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 8;
}
unsigned long REGPARAM2 op_d0fa_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d0fb_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s16 src = get_word(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 6;
}
unsigned long REGPARAM2 op_d0fc_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = get_iword(2);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(4);
  return 4;
}
unsigned long REGPARAM2 op_d100_3(uae_u32 opcode) /* ADDX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uae_s8 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s8)(src)) < 0;
            int flgo = ((uae_s8)(dst)) < 0;
            int flgn = ((uae_s8)(newv)) < 0;
            SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
            SET_NFLG(((uae_s8)(newv)) < 0);
            m68k_dreg(regs, dstreg) =
                (m68k_dreg(regs, dstreg) & ~0xff) | ((newv) & 0xff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d108_3(uae_u32 opcode) /* ADDX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - areg_byteinc[srcreg];
      {
        uae_s8 src = get_byte(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
          {
            uae_s8 dst = get_byte(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
                COPY_CARRY;
                SET_ZFLG(GET_ZFLG & (((uae_s8)(newv)) == 0));
                SET_NFLG(((uae_s8)(newv)) < 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 8;
}
unsigned long REGPARAM2 op_d110_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d118_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) += areg_byteinc[dstreg];
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d120_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - areg_byteinc[dstreg];
        {
          uae_s8 dst = get_byte(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d128_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d130_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d138_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d139_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s8 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s8 dst = get_byte(dsta);
          {
            {
              uae_u32 newv = ((uae_s8)(dst)) + ((uae_s8)(src));
              {
                int flgs = ((uae_s8)(src)) < 0;
                int flgo = ((uae_s8)(dst)) < 0;
                int flgn = ((uae_s8)(newv)) < 0;
                SET_ZFLG(((uae_s8)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u8)(~dst)) < ((uae_u8)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_byte(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_d140_3(uae_u32 opcode) /* ADDX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uae_s16 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s16)(src)) < 0;
            int flgo = ((uae_s16)(dst)) < 0;
            int flgn = ((uae_s16)(newv)) < 0;
            SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
            SET_NFLG(((uae_s16)(newv)) < 0);
            m68k_dreg(regs, dstreg) =
                (m68k_dreg(regs, dstreg) & ~0xffff) | ((newv) & 0xffff);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d148_3(uae_u32 opcode) /* ADDX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 src = get_word(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 2;
          {
            uae_s16 dst = get_word(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
                COPY_CARRY;
                SET_ZFLG(GET_ZFLG & (((uae_s16)(newv)) == 0));
                SET_NFLG(((uae_s16)(newv)) < 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 8;
}
unsigned long REGPARAM2 op_d150_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d158_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) += 2;
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d160_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 2;
        {
          uae_s16 dst = get_word(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d168_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d170_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d178_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d179_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s16 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s16 dst = get_word(dsta);
          {
            {
              uae_u32 newv = ((uae_s16)(dst)) + ((uae_s16)(src));
              {
                int flgs = ((uae_s16)(src)) < 0;
                int flgo = ((uae_s16)(dst)) < 0;
                int flgn = ((uae_s16)(newv)) < 0;
                SET_ZFLG(((uae_s16)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u16)(~dst)) < ((uae_u16)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_word(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_d180_3(uae_u32 opcode) /* ADDX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_dreg(regs, dstreg);
        {
          uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
          {
            int flgs = ((uae_s32)(src)) < 0;
            int flgo = ((uae_s32)(dst)) < 0;
            int flgn = ((uae_s32)(newv)) < 0;
            SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
            SET_CFLG(flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
            COPY_CARRY;
            SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
            SET_NFLG(((uae_s32)(newv)) < 0);
            m68k_dreg(regs, dstreg) = (newv);
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
#endif

#ifdef PART_8
unsigned long REGPARAM2 op_d188_3(uae_u32 opcode) /* ADDX */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uaecptr dsta = m68k_areg(regs, dstreg) - 4;
          {
            uae_s32 dst = get_long(dsta);
            m68k_areg(regs, dstreg) = dsta;
            if (dstreg == 7)
              CHECK_STACK_POINTER_DECREMENT();
            {
              uae_u32 newv = dst + src + (GET_XFLG ? 1 : 0);
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(flgs ^ ((flgs ^ flgo) & (flgo ^ flgn)));
                COPY_CARRY;
                SET_ZFLG(GET_ZFLG & (((uae_s32)(newv)) == 0));
                SET_NFLG(((uae_s32)(newv)) < 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 14;
}
unsigned long REGPARAM2 op_d190_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_d198_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg);
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) += 4;
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_d1a0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) - 4;
        {
          uae_s32 dst = get_long(dsta);
          m68k_areg(regs, dstreg) = dsta;
          if (dstreg == 7)
            CHECK_STACK_POINTER_DECREMENT();
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 10;
}
unsigned long REGPARAM2 op_d1a8_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = m68k_areg(regs, dstreg) + (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_d1b0_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_disp_ea_000(m68k_areg(regs, dstreg), get_iword(2));
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_d1b8_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = (uae_s32)(uae_s16)get_iword(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 12;
}
unsigned long REGPARAM2 op_d1b9_3(uae_u32 opcode) /* ADD */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uaecptr dsta = get_ilong(2);
        {
          uae_s32 dst = get_long(dsta);
          {
            {
              uae_u32 newv = ((uae_s32)(dst)) + ((uae_s32)(src));
              {
                int flgs = ((uae_s32)(src)) < 0;
                int flgo = ((uae_s32)(dst)) < 0;
                int flgn = ((uae_s32)(newv)) < 0;
                SET_ZFLG(((uae_s32)(newv)) == 0);
                SET_VFLG((flgs ^ flgn) & (flgo ^ flgn));
                SET_CFLG(((uae_u32)(~dst)) < ((uae_u32)(src)));
                COPY_CARRY;
                SET_NFLG(flgn != 0);
                put_long(dsta, newv);
              }
            }
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 14;
}
unsigned long REGPARAM2 op_d1c0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_dreg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d1c8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = m68k_areg(regs, srcreg);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_d1d0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d1d8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg);
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) += 4;
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d1e0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) - 4;
      {
        uae_s32 src = get_long(srca);
        m68k_areg(regs, srcreg) = srca;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_d1e8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d1f0_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 srcreg = (opcode & 7);
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d1f8_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d1f9_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = get_ilong(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_d1fa_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr srca = m68k_getpc() + 2;
      srca += (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d1fb_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uaecptr tmppc = m68k_getpc() + 2;
      uaecptr srca = get_disp_ea_000(tmppc, get_iword(2));
      {
        uae_s32 src = get_long(srca);
        {
          uae_s32 dst = m68k_areg(regs, dstreg);
          {
            uae_u32 newv = dst + src;
            m68k_areg(regs, dstreg) = (newv);
            if (dstreg == 7)
              CHECK_STACK_POINTER_INCREMENT();
          }
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_d1fc_3(uae_u32 opcode) /* ADDA */
{
  uae_u32 dstreg = (opcode >> 9) & 7;
  {
    {
      uae_s32 src = get_ilong(2);
      {
        uae_s32 dst = m68k_areg(regs, dstreg);
        {
          uae_u32 newv = dst + src;
          m68k_areg(regs, dstreg) = (newv);
          if (dstreg == 7)
            CHECK_STACK_POINTER_INCREMENT();
        }
      }
    }
  }
  m68k_incpc(6);
  return 6;
}
unsigned long REGPARAM2 op_e000_3(uae_u32 opcode) /* ASR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          uae_u32 sign = (0x80 & val) >> 7;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            val = 0xff & (uae_u32)-sign;
            SET_CFLG(sign);
            COPY_CARRY;
          } else {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
            val |= (0xff << (8 - cnt)) & (uae_u32)-sign;
            val &= 0xff;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e008_3(uae_u32 opcode) /* LSR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            SET_CFLG((cnt == 8) & (val >> 7));
            COPY_CARRY;
            val = 0;
          } else {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e010_3(uae_u32 opcode) /* ROXR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 hival = (val << 1) | GET_XFLG;
              hival <<= (7 - cnt);
              val >>= cnt;
              carry = val & 1;
              val >>= 1;
              val |= hival;
              SET_XFLG(carry);
              val &= 0xff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e018_3(uae_u32 opcode) /* ROR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u8 val = data;
          uae_u32 cmask = 0x80;
          cnt &= 63;
          if (!cnt) {
            CFLG = 0;
          } else {
            uae_u32 carry;
            for (; cnt; --cnt) {
              carry = val & 1;
              val = val >> 1;
              if (carry)
                val |= cmask;
            }
            SET_CFLG(carry);
          }
          SET_NFLG((val & cmask) != 0);
          SET_ZFLG(val == 0);
          SET_VFLG(0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e020_3(uae_u32 opcode) /* ASR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          uae_u32 sign = (0x80 & val) >> 7;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            val = 0xff & (uae_u32)-sign;
            SET_CFLG(sign);
            COPY_CARRY;
          } else if (cnt > 0) {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
            val |= (0xff << (8 - cnt)) & (uae_u32)-sign;
            val &= 0xff;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e028_3(uae_u32 opcode) /* LSR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            SET_CFLG((cnt == 8) & (val >> 7));
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e030_3(uae_u32 opcode) /* ROXR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 36)
            cnt -= 36;
          if (cnt >= 18)
            cnt -= 18;
          if (cnt >= 9)
            cnt -= 9;
          if (cnt > 0) {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 hival = (val << 1) | GET_XFLG;
              hival <<= (7 - cnt);
              val >>= cnt;
              carry = val & 1;
              val >>= 1;
              val |= hival;
              SET_XFLG(carry);
              val &= 0xff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e038_3(uae_u32 opcode) /* ROR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u8 val = data;
          uae_u32 cmask = 0x80;
          cnt &= 63;
          if (!cnt) {
            CFLG = 0;
          } else {
            uae_u32 carry;
            for (; cnt; --cnt) {
              carry = val & 1;
              val = val >> 1;
              if (carry)
                val |= cmask;
            }
            SET_CFLG(carry);
          }
          SET_NFLG((val & cmask) != 0);
          SET_ZFLG(val == 0);
          SET_VFLG(0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e040_3(uae_u32 opcode) /* ASR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = (0x8000 & val) >> 15;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            val = 0xffff & (uae_u32)-sign;
            SET_CFLG(sign);
            COPY_CARRY;
          } else {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
            val |= (0xffff << (16 - cnt)) & (uae_u32)-sign;
            val &= 0xffff;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e048_3(uae_u32 opcode) /* LSR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            SET_CFLG((cnt == 16) & (val >> 15));
            COPY_CARRY;
            val = 0;
          } else {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e050_3(uae_u32 opcode) /* ROXR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 hival = (val << 1) | GET_XFLG;
              hival <<= (15 - cnt);
              val >>= cnt;
              carry = val & 1;
              val >>= 1;
              val |= hival;
              SET_XFLG(carry);
              val &= 0xffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e058_3(uae_u32 opcode) /* ROR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u16 val = data;
          uae_u32 cmask = 0x8000;
          cnt &= 63;
          if (!cnt) {
            CFLG = 0;
          } else {
            uae_u32 carry;
            for (; cnt; --cnt) {
              carry = val & 1;
              val = val >> 1;
              if (carry)
                val |= cmask;
            }
            SET_CFLG(carry);
          }
          SET_NFLG((val & cmask) != 0);
          SET_ZFLG(val == 0);
          SET_VFLG(0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e060_3(uae_u32 opcode) /* ASR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = (0x8000 & val) >> 15;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            val = 0xffff & (uae_u32)-sign;
            SET_CFLG(sign);
            COPY_CARRY;
          } else if (cnt > 0) {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
            val |= (0xffff << (16 - cnt)) & (uae_u32)-sign;
            val &= 0xffff;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e068_3(uae_u32 opcode) /* LSR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            SET_CFLG((cnt == 16) & (val >> 15));
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e070_3(uae_u32 opcode) /* ROXR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 34)
            cnt -= 34;
          if (cnt >= 17)
            cnt -= 17;
          if (cnt > 0) {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 hival = (val << 1) | GET_XFLG;
              hival <<= (15 - cnt);
              val >>= cnt;
              carry = val & 1;
              val >>= 1;
              val |= hival;
              SET_XFLG(carry);
              val &= 0xffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e078_3(uae_u32 opcode) /* ROR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u16 val = data;
          uae_u32 cmask = 0x8000;
          cnt &= 63;
          if (!cnt) {
            CFLG = 0;
          } else {
            uae_u32 carry;
            for (; cnt; --cnt) {
              carry = val & 1;
              val = val >> 1;
              if (carry)
                val |= cmask;
            }
            SET_CFLG(carry);
          }
          SET_NFLG((val & cmask) != 0);
          SET_ZFLG(val == 0);
          SET_VFLG(0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e080_3(uae_u32 opcode) /* ASR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          uae_u32 sign = (0x80000000 & val) >> 31;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            val = 0xffffffff & (uae_u32)-sign;
            SET_CFLG(sign);
            COPY_CARRY;
          } else {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
            val |= (0xffffffff << (32 - cnt)) & (uae_u32)-sign;
            val &= 0xffffffff;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e088_3(uae_u32 opcode) /* LSR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            SET_CFLG((cnt == 32) & (val >> 31));
            COPY_CARRY;
            val = 0;
          } else {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e090_3(uae_u32 opcode) /* ROXR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 hival = (val << 1) | GET_XFLG;
              hival <<= (31 - cnt);
              val >>= cnt;
              carry = val & 1;
              val >>= 1;
              val |= hival;
              SET_XFLG(carry);
              val &= 0xffffffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e098_3(uae_u32 opcode) /* ROR */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          uae_u32 cmask = 0x80000000;
          cnt &= 63;
          if (!cnt) {
            CFLG = 0;
          } else {
            uae_u32 carry;
            for (; cnt; --cnt) {
              carry = val & 1;
              val = val >> 1;
              if (carry)
                val |= cmask;
            }
            SET_CFLG(carry);
          }
          SET_NFLG((val & cmask) != 0);
          SET_ZFLG(val == 0);
          SET_VFLG(0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e0a0_3(uae_u32 opcode) /* ASR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          uae_u32 sign = (0x80000000 & val) >> 31;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            val = 0xffffffff & (uae_u32)-sign;
            SET_CFLG(sign);
            COPY_CARRY;
          } else if (cnt > 0) {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
            val |= (0xffffffff << (32 - cnt)) & (uae_u32)-sign;
            val &= 0xffffffff;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e0a8_3(uae_u32 opcode) /* LSR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            SET_CFLG((cnt == 32) & (val >> 31));
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            val >>= cnt - 1;
            SET_CFLG(val & 1);
            COPY_CARRY;
            val >>= 1;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e0b0_3(uae_u32 opcode) /* ROXR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 33)
            cnt -= 33;
          if (cnt > 0) {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 hival = (val << 1) | GET_XFLG;
              hival <<= (31 - cnt);
              val >>= cnt;
              carry = val & 1;
              val >>= 1;
              val |= hival;
              SET_XFLG(carry);
              val &= 0xffffffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e0b8_3(uae_u32 opcode) /* ROR */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          uae_u32 cmask = 0x80000000;
          cnt &= 63;
          if (!cnt) {
            CFLG = 0;
          } else {
            uae_u32 carry;
            for (; cnt; --cnt) {
              carry = val & 1;
              val = val >> 1;
              if (carry)
                val |= cmask;
            }
            SET_CFLG(carry);
          }
          SET_NFLG((val & cmask) != 0);
          SET_ZFLG(val == 0);
          SET_VFLG(0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e0d0_3(uae_u32 opcode) /* ASRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 cflg = val & 1;
          val = (val >> 1) | sign;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(cflg);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e0d8_3(uae_u32 opcode) /* ASRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 cflg = val & 1;
          val = (val >> 1) | sign;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(cflg);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e0e0_3(uae_u32 opcode) /* ASRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 cflg = val & 1;
          val = (val >> 1) | sign;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(cflg);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e0e8_3(uae_u32 opcode) /* ASRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 cflg = val & 1;
          val = (val >> 1) | sign;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(cflg);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e0f0_3(uae_u32 opcode) /* ASRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 cflg = val & 1;
          val = (val >> 1) | sign;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(cflg);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e0f8_3(uae_u32 opcode) /* ASRW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 cflg = val & 1;
          val = (val >> 1) | sign;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(cflg);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e0f9_3(uae_u32 opcode) /* ASRW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 cflg = val & 1;
          val = (val >> 1) | sign;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(cflg);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_e100_3(uae_u32 opcode) /* ASL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            SET_VFLG(val != 0);
            SET_CFLG(cnt == 8 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else {
            uae_u32 mask = (0xff << (7 - cnt)) & 0xff;
            SET_VFLG((val & mask) != mask && (val & mask) != 0);
            val <<= cnt - 1;
            SET_CFLG((val & 0x80) >> 7);
            COPY_CARRY;
            val <<= 1;
            val &= 0xff;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e108_3(uae_u32 opcode) /* LSL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            SET_CFLG(cnt == 8 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else {
            val <<= (cnt - 1);
            SET_CFLG((val & 0x80) >> 7);
            COPY_CARRY;
            val <<= 1;
            val &= 0xff;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e110_3(uae_u32 opcode) /* ROXL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 loval = val >> (7 - cnt);
              carry = loval & 1;
              val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
              SET_XFLG(carry);
              val &= 0xff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e118_3(uae_u32 opcode) /* ROL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            uae_u32 loval;
            cnt &= 7;
            loval = val >> (8 - cnt);
            val <<= cnt;
            val |= loval;
            val &= 0xff;
            SET_CFLG(val & 1);
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e120_3(uae_u32 opcode) /* ASL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            SET_VFLG(val != 0);
            SET_CFLG(cnt == 8 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            uae_u32 mask = (0xff << (7 - cnt)) & 0xff;
            SET_VFLG((val & mask) != mask && (val & mask) != 0);
            val <<= cnt - 1;
            SET_CFLG((val & 0x80) >> 7);
            COPY_CARRY;
            val <<= 1;
            val &= 0xff;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e128_3(uae_u32 opcode) /* LSL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 8) {
            SET_CFLG(cnt == 8 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            val <<= (cnt - 1);
            SET_CFLG((val & 0x80) >> 7);
            COPY_CARRY;
            val <<= 1;
            val &= 0xff;
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e130_3(uae_u32 opcode) /* ROXL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 36)
            cnt -= 36;
          if (cnt >= 18)
            cnt -= 18;
          if (cnt >= 9)
            cnt -= 9;
          if (cnt > 0) {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 loval = val >> (7 - cnt);
              carry = loval & 1;
              val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
              SET_XFLG(carry);
              val &= 0xff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e138_3(uae_u32 opcode) /* ROL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s8 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s8 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u8)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt > 0) {
            uae_u32 loval;
            cnt &= 7;
            loval = val >> (8 - cnt);
            val <<= cnt;
            val |= loval;
            val &= 0xff;
            SET_CFLG(val & 1);
          }
          SET_ZFLG(((uae_s8)(val)) == 0);
          SET_NFLG(((uae_s8)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xff) | ((val) & 0xff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e140_3(uae_u32 opcode) /* ASL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            SET_VFLG(val != 0);
            SET_CFLG(cnt == 16 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else {
            uae_u32 mask = (0xffff << (15 - cnt)) & 0xffff;
            SET_VFLG((val & mask) != mask && (val & mask) != 0);
            val <<= cnt - 1;
            SET_CFLG((val & 0x8000) >> 15);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffff;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e148_3(uae_u32 opcode) /* LSL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            SET_CFLG(cnt == 16 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else {
            val <<= (cnt - 1);
            SET_CFLG((val & 0x8000) >> 15);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffff;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e150_3(uae_u32 opcode) /* ROXL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 loval = val >> (15 - cnt);
              carry = loval & 1;
              val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
              SET_XFLG(carry);
              val &= 0xffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e158_3(uae_u32 opcode) /* ROL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            uae_u32 loval;
            cnt &= 15;
            loval = val >> (16 - cnt);
            val <<= cnt;
            val |= loval;
            val &= 0xffff;
            SET_CFLG(val & 1);
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e160_3(uae_u32 opcode) /* ASL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            SET_VFLG(val != 0);
            SET_CFLG(cnt == 16 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            uae_u32 mask = (0xffff << (15 - cnt)) & 0xffff;
            SET_VFLG((val & mask) != mask && (val & mask) != 0);
            val <<= cnt - 1;
            SET_CFLG((val & 0x8000) >> 15);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffff;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e168_3(uae_u32 opcode) /* LSL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 16) {
            SET_CFLG(cnt == 16 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            val <<= (cnt - 1);
            SET_CFLG((val & 0x8000) >> 15);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffff;
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e170_3(uae_u32 opcode) /* ROXL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 34)
            cnt -= 34;
          if (cnt >= 17)
            cnt -= 17;
          if (cnt > 0) {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 loval = val >> (15 - cnt);
              carry = loval & 1;
              val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
              SET_XFLG(carry);
              val &= 0xffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e178_3(uae_u32 opcode) /* ROL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s16 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s16 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = (uae_u16)data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt > 0) {
            uae_u32 loval;
            cnt &= 15;
            loval = val >> (16 - cnt);
            val <<= cnt;
            val |= loval;
            val &= 0xffff;
            SET_CFLG(val & 1);
          }
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          m68k_dreg(regs, dstreg) =
              (m68k_dreg(regs, dstreg) & ~0xffff) | ((val) & 0xffff);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e180_3(uae_u32 opcode) /* ASL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            SET_VFLG(val != 0);
            SET_CFLG(cnt == 32 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else {
            uae_u32 mask = (0xffffffff << (31 - cnt)) & 0xffffffff;
            SET_VFLG((val & mask) != mask && (val & mask) != 0);
            val <<= cnt - 1;
            SET_CFLG((val & 0x80000000) >> 31);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffffffff;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e188_3(uae_u32 opcode) /* LSL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            SET_CFLG(cnt == 32 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else {
            val <<= (cnt - 1);
            SET_CFLG((val & 0x80000000) >> 31);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffffffff;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e190_3(uae_u32 opcode) /* ROXL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 loval = val >> (31 - cnt);
              carry = loval & 1;
              val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
              SET_XFLG(carry);
              val &= 0xffffffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e198_3(uae_u32 opcode) /* ROL */
{
  uae_u32 srcreg = imm8_table[((opcode >> 9) & 7)];
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_u32 cnt = srcreg;
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          {
            uae_u32 loval;
            cnt &= 31;
            loval = val >> (32 - cnt);
            val <<= cnt;
            val |= loval;
            val &= 0xffffffff;
            SET_CFLG(val & 1);
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e1a0_3(uae_u32 opcode) /* ASL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            SET_VFLG(val != 0);
            SET_CFLG(cnt == 32 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            uae_u32 mask = (0xffffffff << (31 - cnt)) & 0xffffffff;
            SET_VFLG((val & mask) != mask && (val & mask) != 0);
            val <<= cnt - 1;
            SET_CFLG((val & 0x80000000) >> 31);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffffffff;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e1a8_3(uae_u32 opcode) /* LSL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 32) {
            SET_CFLG(cnt == 32 ? val & 1 : 0);
            COPY_CARRY;
            val = 0;
          } else if (cnt > 0) {
            val <<= (cnt - 1);
            SET_CFLG((val & 0x80000000) >> 31);
            COPY_CARRY;
            val <<= 1;
            val &= 0xffffffff;
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e1b0_3(uae_u32 opcode) /* ROXL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt >= 33)
            cnt -= 33;
          if (cnt > 0) {
            cnt--;
            {
              uae_u32 carry;
              uae_u32 loval = val >> (31 - cnt);
              carry = loval & 1;
              val = (((val << 1) | GET_XFLG) << cnt) | (loval >> 1);
              SET_XFLG(carry);
              val &= 0xffffffff;
            }
          }
          SET_CFLG(GET_XFLG);
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e1b8_3(uae_u32 opcode) /* ROL */
{
  uae_u32 srcreg = ((opcode >> 9) & 7);
  uae_u32 dstreg = opcode & 7;
  {
    {
      uae_s32 cnt = m68k_dreg(regs, srcreg);
      {
        uae_s32 data = m68k_dreg(regs, dstreg);
        {
          uae_u32 val = data;
          cnt &= 63;
          CLEAR_CZNV;
          if (cnt > 0) {
            uae_u32 loval;
            cnt &= 31;
            loval = val >> (32 - cnt);
            val <<= cnt;
            val |= loval;
            val &= 0xffffffff;
            SET_CFLG(val & 1);
          }
          SET_ZFLG(((uae_s32)(val)) == 0);
          SET_NFLG(((uae_s32)(val)) < 0);
          m68k_dreg(regs, dstreg) = (val);
#if HAS_PROFILING
          if (gProfilingEnabled)
            ProfileIncrementClock(2 * cnt);
#endif
        }
      }
    }
  }
  m68k_incpc(2);
  return 2;
}
unsigned long REGPARAM2 op_e1d0_3(uae_u32 opcode) /* ASLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 sign2;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          sign2 = 0x8000 & val;
          SET_CFLG(sign != 0);
          COPY_CARRY;
          SET_VFLG(GET_VFLG | (sign2 != sign));
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e1d8_3(uae_u32 opcode) /* ASLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 sign2;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          sign2 = 0x8000 & val;
          SET_CFLG(sign != 0);
          COPY_CARRY;
          SET_VFLG(GET_VFLG | (sign2 != sign));
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e1e0_3(uae_u32 opcode) /* ASLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 sign2;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          sign2 = 0x8000 & val;
          SET_CFLG(sign != 0);
          COPY_CARRY;
          SET_VFLG(GET_VFLG | (sign2 != sign));
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e1e8_3(uae_u32 opcode) /* ASLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 sign2;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          sign2 = 0x8000 & val;
          SET_CFLG(sign != 0);
          COPY_CARRY;
          SET_VFLG(GET_VFLG | (sign2 != sign));
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e1f0_3(uae_u32 opcode) /* ASLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 sign2;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          sign2 = 0x8000 & val;
          SET_CFLG(sign != 0);
          COPY_CARRY;
          SET_VFLG(GET_VFLG | (sign2 != sign));
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e1f8_3(uae_u32 opcode) /* ASLW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 sign2;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          sign2 = 0x8000 & val;
          SET_CFLG(sign != 0);
          COPY_CARRY;
          SET_VFLG(GET_VFLG | (sign2 != sign));
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e1f9_3(uae_u32 opcode) /* ASLW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 sign = 0x8000 & val;
          uae_u32 sign2;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          sign2 = 0x8000 & val;
          SET_CFLG(sign != 0);
          COPY_CARRY;
          SET_VFLG(GET_VFLG | (sign2 != sign));
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_e2d0_3(uae_u32 opcode) /* LSRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 carry = val & 1;
          val >>= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e2d8_3(uae_u32 opcode) /* LSRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 carry = val & 1;
          val >>= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e2e0_3(uae_u32 opcode) /* LSRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 carry = val & 1;
          val >>= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e2e8_3(uae_u32 opcode) /* LSRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 carry = val & 1;
          val >>= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e2f0_3(uae_u32 opcode) /* LSRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 carry = val & 1;
          val >>= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e2f8_3(uae_u32 opcode) /* LSRW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 carry = val & 1;
          val >>= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e2f9_3(uae_u32 opcode) /* LSRW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u32 val = (uae_u16)data;
          uae_u32 carry = val & 1;
          val >>= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_e3d0_3(uae_u32 opcode) /* LSLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e3d8_3(uae_u32 opcode) /* LSLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e3e0_3(uae_u32 opcode) /* LSLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e3e8_3(uae_u32 opcode) /* LSLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e3f0_3(uae_u32 opcode) /* LSLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e3f8_3(uae_u32 opcode) /* LSLW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e3f9_3(uae_u32 opcode) /* LSLW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_e4d0_3(uae_u32 opcode) /* ROXRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (GET_XFLG)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e4d8_3(uae_u32 opcode) /* ROXRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (GET_XFLG)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e4e0_3(uae_u32 opcode) /* ROXRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (GET_XFLG)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e4e8_3(uae_u32 opcode) /* ROXRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (GET_XFLG)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e4f0_3(uae_u32 opcode) /* ROXRW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (GET_XFLG)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e4f8_3(uae_u32 opcode) /* ROXRW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (GET_XFLG)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e4f9_3(uae_u32 opcode) /* ROXRW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (GET_XFLG)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_e5d0_3(uae_u32 opcode) /* ROXLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (GET_XFLG)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e5d8_3(uae_u32 opcode) /* ROXLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (GET_XFLG)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e5e0_3(uae_u32 opcode) /* ROXLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (GET_XFLG)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e5e8_3(uae_u32 opcode) /* ROXLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (GET_XFLG)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e5f0_3(uae_u32 opcode) /* ROXLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (GET_XFLG)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e5f8_3(uae_u32 opcode) /* ROXLW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (GET_XFLG)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e5f9_3(uae_u32 opcode) /* ROXLW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (GET_XFLG)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          COPY_CARRY;
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_e6d0_3(uae_u32 opcode) /* RORW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (carry)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e6d8_3(uae_u32 opcode) /* RORW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (carry)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e6e0_3(uae_u32 opcode) /* RORW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (carry)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e6e8_3(uae_u32 opcode) /* RORW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (carry)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e6f0_3(uae_u32 opcode) /* RORW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (carry)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e6f8_3(uae_u32 opcode) /* RORW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (carry)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e6f9_3(uae_u32 opcode) /* RORW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 1;
          val >>= 1;
          if (carry)
            val |= 0x8000;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
unsigned long REGPARAM2 op_e7d0_3(uae_u32 opcode) /* ROLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (carry)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e7d8_3(uae_u32 opcode) /* ROLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg);
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) += 2;
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (carry)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e7e0_3(uae_u32 opcode) /* ROLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) - 2;
      {
        uae_s16 data = get_word(dataa);
        m68k_areg(regs, srcreg) = dataa;
        if (srcreg == 7)
          CHECK_STACK_POINTER_DECREMENT();
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (carry)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(2);
  return 6;
}
unsigned long REGPARAM2 op_e7e8_3(uae_u32 opcode) /* ROLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = m68k_areg(regs, srcreg) + (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (carry)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e7f0_3(uae_u32 opcode) /* ROLW */
{
  uae_u32 srcreg = (opcode & 7);
  {
    {
      uaecptr dataa = get_disp_ea_000(m68k_areg(regs, srcreg), get_iword(2));
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (carry)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e7f8_3(uae_u32 opcode) /* ROLW */
{
  {
    {
      uaecptr dataa = (uae_s32)(uae_s16)get_iword(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (carry)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(4);
  return 8;
}
unsigned long REGPARAM2 op_e7f9_3(uae_u32 opcode) /* ROLW */
{
  {
    {
      uaecptr dataa = get_ilong(2);
      {
        uae_s16 data = get_word(dataa);
        {
          uae_u16 val = data;
          uae_u32 carry = val & 0x8000;
          val <<= 1;
          if (carry)
            val |= 1;
          CLEAR_CZNV;
          SET_ZFLG(((uae_s16)(val)) == 0);
          SET_NFLG(((uae_s16)(val)) < 0);
          SET_CFLG(carry >> 15);
          put_word(dataa, val);
        }
      }
    }
  }
  m68k_incpc(6);
  return 10;
}
#endif
