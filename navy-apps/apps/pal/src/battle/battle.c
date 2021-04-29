/* -*- mode: c; tab-width: 4; c-basic-offset: 3; c-file-style: "linux" -*- */
//
// Copyright (c) 2009, Wei Mingzhi <whistler_wmz@users.sf.net>.
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "main.h"
#include "FLOAT.h"

BATTLE          g_Battle;

WORD
g_rgPlayerPos[3][3][2] = {
   {{240, 170}},                         // one player
   {{200, 176}, {256, 152}},             // two players
   {{180, 180}, {234, 170}, {270, 146}}  // three players
};

VOID
PAL_BattleMakeScene(
   VOID
)
/*++
  Purpose:

    Generate the battle scene into the scene buffer.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   int          i;
   PAL_POS      pos;
   LPBYTE       pSrc, pDst;
   BYTE         shifttable[256];

   //
   // Draw the background
   //
   pSrc = g_Battle.lpBackground->pixels;
   pDst = g_Battle.lpSceneBuf->pixels;
   PAL_MakeColorShiftTable(shifttable, g_Battle.sBackgroundColorShift);

   for (i = 0; i < g_Battle.lpSceneBuf->pitch * g_Battle.lpSceneBuf->h; i++)
   {
      *pDst = shifttable[*pSrc];

      ++pSrc;
      ++pDst;
   }

   PAL_ApplyWave(g_Battle.lpSceneBuf);

   //
   // Draw the enemies
   //
   for (i = g_Battle.wMaxEnemyIndex; i >= 0; i--)
   {
      pos = g_Battle.rgEnemy[i].pos;

      if (g_Battle.rgEnemy[i].rgwStatus[kStatusConfused] > 0 &&
         g_Battle.rgEnemy[i].rgwStatus[kStatusSleep] == 0 &&
         g_Battle.rgEnemy[i].rgwStatus[kStatusParalyzed] == 0)
      {
         //
         // Enemy is confused
         //
         pos = PAL_XY(PAL_X(pos) + RandomLong(-1, 1), PAL_Y(pos));
      }

      pos = PAL_XY(PAL_X(pos) - PAL_RLEGetWidth(PAL_SpriteGetFrame(g_Battle.rgEnemy[i].lpSprite, g_Battle.rgEnemy[i].wCurrentFrame)) / 2,
         PAL_Y(pos) - PAL_RLEGetHeight(PAL_SpriteGetFrame(g_Battle.rgEnemy[i].lpSprite, g_Battle.rgEnemy[i].wCurrentFrame)));

      if (g_Battle.rgEnemy[i].wObjectID != 0)
      {
         if (g_Battle.rgEnemy[i].iColorShift)
         {
            PAL_RLEBlitWithColorShift(PAL_SpriteGetFrame(g_Battle.rgEnemy[i].lpSprite, g_Battle.rgEnemy[i].wCurrentFrame),
               g_Battle.lpSceneBuf, pos, g_Battle.rgEnemy[i].iColorShift);
         }
         else
         {
            PAL_RLEBlitToSurface(PAL_SpriteGetFrame(g_Battle.rgEnemy[i].lpSprite, g_Battle.rgEnemy[i].wCurrentFrame),
               g_Battle.lpSceneBuf, pos);
         }
      }
   }

   if (g_Battle.lpSummonSprite != NULL)
   {
      //
      // Draw the summoned god
      //
      pos = PAL_XY(PAL_X(g_Battle.posSummon) - PAL_RLEGetWidth(PAL_SpriteGetFrame(g_Battle.lpSummonSprite, g_Battle.iSummonFrame)) / 2,
         PAL_Y(g_Battle.posSummon) - PAL_RLEGetHeight(PAL_SpriteGetFrame(g_Battle.lpSummonSprite, g_Battle.iSummonFrame)));

      PAL_RLEBlitToSurface(PAL_SpriteGetFrame(g_Battle.lpSummonSprite, g_Battle.iSummonFrame),
         g_Battle.lpSceneBuf, pos);
   }
   else
   {
      //
      // Draw the players
      //
      for (i = gpGlobals->wMaxPartyMemberIndex; i >= 0; i--)
      {
         pos = g_Battle.rgPlayer[i].pos;

         if (gpGlobals->rgPlayerStatus[gpGlobals->rgParty[i].wPlayerRole][kStatusConfused] != 0 &&
            gpGlobals->rgPlayerStatus[gpGlobals->rgParty[i].wPlayerRole][kStatusSleep] == 0 &&
            gpGlobals->rgPlayerStatus[gpGlobals->rgParty[i].wPlayerRole][kStatusParalyzed] == 0 &&
            gpGlobals->g.PlayerRoles.rgwHP[gpGlobals->rgParty[i].wPlayerRole] > 0)
         {
            //
            // Player is confused
            //
            continue;
         }

         pos = PAL_XY(PAL_X(pos) - PAL_RLEGetWidth(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame)) / 2,
            PAL_Y(pos) - PAL_RLEGetHeight(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame)));

         if (g_Battle.rgPlayer[i].iColorShift != 0)
         {
            PAL_RLEBlitWithColorShift(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame),
               g_Battle.lpSceneBuf, pos, g_Battle.rgPlayer[i].iColorShift);
         }
         else if (g_Battle.iHidingTime == 0)
         {
            PAL_RLEBlitToSurface(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame),
               g_Battle.lpSceneBuf, pos);
         }
      }

      //
      // Confused players should be drawn on top of normal players
      //
      for (i = gpGlobals->wMaxPartyMemberIndex; i >= 0; i--)
      {
         if (gpGlobals->rgPlayerStatus[gpGlobals->rgParty[i].wPlayerRole][kStatusConfused] != 0 &&
            gpGlobals->rgPlayerStatus[gpGlobals->rgParty[i].wPlayerRole][kStatusSleep] == 0 &&
            gpGlobals->rgPlayerStatus[gpGlobals->rgParty[i].wPlayerRole][kStatusParalyzed] == 0 &&
            gpGlobals->g.PlayerRoles.rgwHP[gpGlobals->rgParty[i].wPlayerRole] > 0)
         {
            //
            // Player is confused
            //
            pos = PAL_XY(PAL_X(g_Battle.rgPlayer[i].pos), PAL_Y(g_Battle.rgPlayer[i].pos) + RandomLong(-1, 1));
            pos = PAL_XY(PAL_X(pos) - PAL_RLEGetWidth(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame)) / 2,
               PAL_Y(pos) - PAL_RLEGetHeight(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame)));

            if (g_Battle.rgPlayer[i].iColorShift != 0)
            {
               PAL_RLEBlitWithColorShift(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame),
                  g_Battle.lpSceneBuf, pos, g_Battle.rgPlayer[i].iColorShift);
            }
            else if (g_Battle.iHidingTime == 0)
            {
               PAL_RLEBlitToSurface(PAL_SpriteGetFrame(g_Battle.rgPlayer[i].lpSprite, g_Battle.rgPlayer[i].wCurrentFrame),
                  g_Battle.lpSceneBuf, pos);
            }
         }
      }
   }
}

VOID
PAL_BattleBackupScene(
   VOID
)
/*++
  Purpose:

    Backup the scene buffer.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   SDL_BlitSurface(g_Battle.lpSceneBuf, NULL, gpScreenBak, NULL);
}

VOID
PAL_BattleFadeScene(
   VOID
)
/*++
  Purpose:

    Fade in the scene of battle.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   int               i, j, k;
   DWORD             time;
   BYTE              a, b;
   const int         rgIndex[6] = {0, 3, 1, 5, 2, 4};
   
   time = SDL_GetTicks();

   for (i = 0; i < 12; i++)
   {
      for (j = 0; j < 6; j++)
      {
         PAL_ProcessEvent();
         while (SDL_GetTicks() <= time)
         {
            PAL_ProcessEvent();
            SDL_Delay(1);
         }
         time = SDL_GetTicks() + 16;

         //
         // Blend the pixels in the 2 buffers, and put the result into the
         // backup buffer
         //
         for (k = rgIndex[j]; k < gpScreen->pitch * gpScreen->h; k += 6)
         {
            a = ((LPBYTE)(g_Battle.lpSceneBuf->pixels))[k];
            b = ((LPBYTE)(gpScreenBak->pixels))[k];

            if (i > 0)
            {
               if ((a & 0x0F) > (b & 0x0F))
               {
                  b++;
               }
               else if ((a & 0x0F) < (b & 0x0F))
               {
                  b--;
               }
            }

            ((LPBYTE)(gpScreenBak->pixels))[k] = ((a & 0xF0) | (b & 0x0F));
         }

         //
         // Draw the backup buffer to the screen
         //
         SDL_BlitSurface(gpScreenBak, NULL, gpScreen, NULL);

         PAL_BattleUIUpdate();
         VIDEO_UpdateScreen(NULL);
      }
   }

   //
   // Draw the result buffer to the screen as the final step
   //
   SDL_BlitSurface(g_Battle.lpSceneBuf, NULL, gpScreen, NULL);
   PAL_BattleUIUpdate();

   VIDEO_UpdateScreen(NULL);
}

static BATTLERESULT
PAL_BattleMain(
   VOID
)
/*++
  Purpose:

    The main battle routine.

  Parameters:

    None.

  Return value:

    The result of the battle.

--*/
{
   int         i;
   DWORD       dwTime;
   
   VIDEO_BackupScreen();

   //
   // Generate the scene and draw the scene to the screen buffer
   //
   PAL_BattleMakeScene();
   SDL_BlitSurface(g_Battle.lpSceneBuf, NULL, gpScreen, NULL);

   //
   // Fade out the music and delay for a while
   //
   PAL_PlayMUS(0, FALSE, 1);
   UTIL_Delay(200);

   //
   // Switch the screen
   //
   VIDEO_SwitchScreen(5);

   //
   // Play the battle music
   //
   PAL_PlayMUS(gpGlobals->wNumBattleMusic, TRUE, 0);

   //
   // Fade in the screen when needed
   //
   if (gpGlobals->fNeedToFadeIn)
   {
      PAL_FadeIn(gpGlobals->wNumPalette, gpGlobals->fNightPalette, 1);
      gpGlobals->fNeedToFadeIn = FALSE;
   }

   //
   // Run the pre-battle scripts for each enemies
   //
   for (i = 0; i <= g_Battle.wMaxEnemyIndex; i++)
   {
      g_Battle.rgEnemy[i].wScriptOnTurnStart =
         PAL_RunTriggerScript(g_Battle.rgEnemy[i].wScriptOnTurnStart, i);

      if (g_Battle.BattleResult != kBattleResultPreBattle)
      {
         break;
      }
   }

   if (g_Battle.BattleResult == kBattleResultPreBattle)
   {
      g_Battle.BattleResult = kBattleResultOnGoing;
   }

#ifndef PAL_CLASSIC
   PAL_UpdateTimeChargingUnit();
#endif

   dwTime = SDL_GetTicks();

   PAL_ClearKeyState();

   //
   // Run the main battle loop.
   //
   while (TRUE)
   {
      //
      // Break out if the battle ended.
      //
      if (g_Battle.BattleResult != kBattleResultOnGoing)
      {
         break;
      }

      //
      // Wait for the time of one frame. Accept input here.
      //
      PAL_ProcessEvent();
      while (SDL_GetTicks() <= dwTime)
      {
         PAL_ProcessEvent();
         SDL_Delay(1);
      }

      //
      // Set the time of the next frame.
      //
      dwTime = SDL_GetTicks() + BATTLE_FRAME_TIME;

      //
      // Run the main frame routine.
      //
      PAL_BattleStartFrame();

      //
      // Update the screen.
      //
      VIDEO_UpdateScreen(NULL);
   }

   //
   // Return the battle result
   //
   return g_Battle.BattleResult;
}

static VOID
PAL_FreeBattleSprites(
   VOID
)
/*++
  Purpose:

    Free all the loaded sprites.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   int         i;

   //
   // Free all the loaded sprites
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      if (g_Battle.rgPlayer[i].lpSprite != NULL)
      {
         free(g_Battle.rgPlayer[i].lpSprite);
      }
      g_Battle.rgPlayer[i].lpSprite = NULL;
   }

   for (i = 0; i <= g_Battle.wMaxEnemyIndex; i++)
   {
      if (g_Battle.rgEnemy[i].lpSprite != NULL)
      {
         free(g_Battle.rgEnemy[i].lpSprite);
      }
      g_Battle.rgEnemy[i].lpSprite = NULL;
   }

   if (g_Battle.lpSummonSprite != NULL)
   {
      free(g_Battle.lpSummonSprite);
   }
   g_Battle.lpSummonSprite = NULL;
}

VOID
PAL_LoadBattleSprites(
   VOID
)
/*++
  Purpose:

    Load all the loaded sprites.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   int           i, l, x, y, s;
   FILE         *fp;

   PAL_FreeBattleSprites();

   fp = UTIL_OpenRequiredFile("abc.mkf");

   //
   // Load battle sprites for players
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      s = PAL_GetPlayerBattleSprite(gpGlobals->rgParty[i].wPlayerRole);

      l = PAL_MKFGetDecompressedSize(s, gpGlobals->f.fpF);

      if (l <= 0)
      {
         continue;
      }

      g_Battle.rgPlayer[i].lpSprite = UTIL_calloc(l, 1);

      PAL_MKFDecompressChunk(g_Battle.rgPlayer[i].lpSprite, l,
         s, gpGlobals->f.fpF);

      //
      // Set the default position for this player
      //
      x = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][i][0];
      y = g_rgPlayerPos[gpGlobals->wMaxPartyMemberIndex][i][1];

      g_Battle.rgPlayer[i].posOriginal = PAL_XY(x, y);
      g_Battle.rgPlayer[i].pos = PAL_XY(x, y);
   }

   //
   // Load battle sprites for enemies
   //
   for (i = 0; i < MAX_ENEMIES_IN_TEAM; i++)
   {
      if (g_Battle.rgEnemy[i].wObjectID == 0)
      {
         continue;
      }

      l = PAL_MKFGetDecompressedSize(
         gpGlobals->g.rgObject[g_Battle.rgEnemy[i].wObjectID].enemy.wEnemyID, fp);

      if (l <= 0)
      {
         continue;
      }

      g_Battle.rgEnemy[i].lpSprite = UTIL_calloc(l, 1);

      PAL_MKFDecompressChunk(g_Battle.rgEnemy[i].lpSprite, l,
         gpGlobals->g.rgObject[g_Battle.rgEnemy[i].wObjectID].enemy.wEnemyID, fp);

      //
      // Set the default position for this enemy
      //
      x = gpGlobals->g.EnemyPos.pos[i][g_Battle.wMaxEnemyIndex].x;
      y = gpGlobals->g.EnemyPos.pos[i][g_Battle.wMaxEnemyIndex].y;

      y += g_Battle.rgEnemy[i].e.wYPosOffset;

      g_Battle.rgEnemy[i].posOriginal = PAL_XY(x, y);
      g_Battle.rgEnemy[i].pos = PAL_XY(x, y);
   }

   fclose(fp);
}

static VOID
PAL_LoadBattleBackground(
   VOID
)
/*++
  Purpose:

    Load the screen background picture of the battle.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   PAL_LARGE BYTE           buf[320 * 200];

   //
   // Create the surface
   //
   g_Battle.lpBackground =
      SDL_CreateRGBSurface(gpScreen->flags & ~SDL_HWSURFACE, 320, 200, 8,
      gpScreen->format->Rmask, gpScreen->format->Gmask,
      gpScreen->format->Bmask, gpScreen->format->Amask);

   if (g_Battle.lpBackground == NULL)
   {
      TerminateOnError("PAL_LoadBattleBackground(): failed to create surface!");
   }
#if SDL_VERSION_ATLEAST(2, 0, 0)
   SDL_SetSurfacePalette(g_Battle.lpBackground, gpScreen->format->palette);
#else
   SDL_SetPalette(g_Battle.lpBackground, SDL_PHYSPAL | SDL_LOGPAL, VIDEO_GetPalette(), 0, 256);
#endif
   //
   // Load the picture
   //
   PAL_MKFDecompressChunk(buf, 320 * 200, gpGlobals->wNumBattleField, gpGlobals->f.fpFBP);

   //
   // Draw the picture to the surface.
   //
   PAL_FBPBlitToSurface(buf, g_Battle.lpBackground);
}

static VOID
PAL_BattleWon(
   VOID
)
/*++
  Purpose:

    Show the "you win" message and add the experience points for players.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   const SDL_Rect   rect = {65, 60, 200, 100};
   const SDL_Rect   rect1 = {80, 0, 180, 200};

   int              i, j, iTotalCount;
   DWORD            dwExp;
   WORD             w;
   BOOL             fLevelUp;
   PLAYERROLES      OrigPlayerRoles;

   //
   // Backup the initial player stats
   //
   OrigPlayerRoles = gpGlobals->g.PlayerRoles;

   if (g_Battle.iExpGained > 0)
   {
      //
      // Play the "battle win" music
      //
      PAL_PlayMUS(g_Battle.fIsBoss ? 2 : 3, FALSE, 0);

      //
      // Show the message about the total number of exp. and cash gained
      //
      PAL_CreateSingleLineBox(PAL_XY(83, 60), 8, FALSE);
      PAL_CreateSingleLineBox(PAL_XY(65, 105), 10, FALSE);

      PAL_DrawText(PAL_GetWord(BATTLEWIN_GETEXP_LABEL), PAL_XY(95, 70), 0, FALSE, FALSE);
      PAL_DrawText(PAL_GetWord(BATTLEWIN_BEATENEMY_LABEL), PAL_XY(77, 115), 0, FALSE, FALSE);
      PAL_DrawText(PAL_GetWord(BATTLEWIN_DOLLAR_LABEL), PAL_XY(197, 115), 0, FALSE, FALSE);

      PAL_DrawNumber(g_Battle.iExpGained, 5, PAL_XY(182, 74), kNumColorYellow, kNumAlignRight);
      PAL_DrawNumber(g_Battle.iCashGained, 5, PAL_XY(162, 119), kNumColorYellow, kNumAlignMid);

      VIDEO_UpdateScreen(&rect);
      PAL_WaitForKey(g_Battle.fIsBoss ? 5500 : 3000);
   }

   //
   // Add the cash value
   //
   gpGlobals->dwCash += g_Battle.iCashGained;

   //
   // Add the experience points for each players
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      fLevelUp = FALSE;

      w = gpGlobals->rgParty[i].wPlayerRole;
      if (gpGlobals->g.PlayerRoles.rgwHP[w] == 0)
      {
         continue; // don't care about dead players
      }

      dwExp = gpGlobals->Exp.rgPrimaryExp[w].wExp;
      dwExp += g_Battle.iExpGained;

      if (gpGlobals->g.PlayerRoles.rgwLevel[w] > MAX_LEVELS)
      {
         gpGlobals->g.PlayerRoles.rgwLevel[w] = MAX_LEVELS;
      }

      while (dwExp >= gpGlobals->g.rgLevelUpExp[gpGlobals->g.PlayerRoles.rgwLevel[w]])
      {
         dwExp -= gpGlobals->g.rgLevelUpExp[gpGlobals->g.PlayerRoles.rgwLevel[w]];

         if (gpGlobals->g.PlayerRoles.rgwLevel[w] < MAX_LEVELS)
         {
            fLevelUp = TRUE;
            PAL_PlayerLevelUp(w, 1);

            gpGlobals->g.PlayerRoles.rgwHP[w] = gpGlobals->g.PlayerRoles.rgwMaxHP[w];
            gpGlobals->g.PlayerRoles.rgwMP[w] = gpGlobals->g.PlayerRoles.rgwMaxMP[w];
         }
      }

      gpGlobals->Exp.rgPrimaryExp[w].wExp = (WORD)dwExp;

      if (fLevelUp)
      {
         //
         // Player has gained a level. Show the message
         //
         PAL_CreateSingleLineBox(PAL_XY(80, 0), 10, FALSE);
         PAL_CreateBox(PAL_XY(82, 32), 7, 8, 1, FALSE);

         PAL_DrawText(PAL_GetWord(gpGlobals->g.PlayerRoles.rgwName[w]), PAL_XY(110, 10), 0,
            FALSE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_LEVEL), PAL_XY(110 + 16 * 3, 10), 0, FALSE, FALSE);
         PAL_DrawText(PAL_GetWord(BATTLEWIN_LEVELUP_LABEL), PAL_XY(110 + 16 * 5, 10), 0, FALSE, FALSE);

         for (j = 0; j < 8; j++)
         {
            PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_ARROW),
               gpScreen, PAL_XY(183, 48 + 18 * j));
         }

         PAL_DrawText(PAL_GetWord(STATUS_LABEL_LEVEL), PAL_XY(100, 44), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_HP), PAL_XY(100, 62), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_MP), PAL_XY(100, 80), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_ATTACKPOWER), PAL_XY(100, 98), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_MAGICPOWER), PAL_XY(100, 116), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_RESISTANCE), PAL_XY(100, 134), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_DEXTERITY), PAL_XY(100, 152), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);
         PAL_DrawText(PAL_GetWord(STATUS_LABEL_FLEERATE), PAL_XY(100, 170), BATTLEWIN_LEVELUP_LABEL_COLOR,
            TRUE, FALSE);

         //
         // Draw the original stats and stats after level up
         //
         PAL_DrawNumber(OrigPlayerRoles.rgwLevel[w], 4, PAL_XY(133, 47),
            kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwLevel[w], 4, PAL_XY(195, 47),
            kNumColorYellow, kNumAlignRight);

         PAL_DrawNumber(OrigPlayerRoles.rgwHP[w], 4, PAL_XY(133, 64),
            kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(OrigPlayerRoles.rgwMaxHP[w], 4, PAL_XY(154, 68),
            kNumColorBlue, kNumAlignRight);
         PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_SLASH), gpScreen,
            PAL_XY(156, 66));
         PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwHP[w], 4, PAL_XY(195, 64),
            kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwMaxHP[w], 4, PAL_XY(216, 68),
            kNumColorBlue, kNumAlignRight);
         PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_SLASH), gpScreen,
            PAL_XY(218, 66));

         PAL_DrawNumber(OrigPlayerRoles.rgwMP[w], 4, PAL_XY(133, 82),
            kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(OrigPlayerRoles.rgwMaxMP[w], 4, PAL_XY(154, 86),
            kNumColorBlue, kNumAlignRight);
         PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_SLASH), gpScreen,
            PAL_XY(156, 84));
         PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwMP[w], 4, PAL_XY(195, 82),
            kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(gpGlobals->g.PlayerRoles.rgwMaxMP[w], 4, PAL_XY(216, 86),
            kNumColorBlue, kNumAlignRight);
         PAL_RLEBlitToSurface(PAL_SpriteGetFrame(gpSpriteUI, SPRITENUM_SLASH), gpScreen,
            PAL_XY(218, 84));

         PAL_DrawNumber(OrigPlayerRoles.rgwAttackStrength[w] + PAL_GetPlayerAttackStrength(w) -
            gpGlobals->g.PlayerRoles.rgwAttackStrength[w],
            4, PAL_XY(133, 101), kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(PAL_GetPlayerAttackStrength(w), 4, PAL_XY(195, 101),
            kNumColorYellow, kNumAlignRight);

         PAL_DrawNumber(OrigPlayerRoles.rgwMagicStrength[w] + PAL_GetPlayerMagicStrength(w) -
            gpGlobals->g.PlayerRoles.rgwMagicStrength[w],
            4, PAL_XY(133, 119), kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(PAL_GetPlayerMagicStrength(w), 4, PAL_XY(195, 119),
            kNumColorYellow, kNumAlignRight);

         PAL_DrawNumber(OrigPlayerRoles.rgwDefense[w] + PAL_GetPlayerDefense(w) -
            gpGlobals->g.PlayerRoles.rgwDefense[w],
            4, PAL_XY(133, 137), kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(PAL_GetPlayerDefense(w), 4, PAL_XY(195, 137),
            kNumColorYellow, kNumAlignRight);

         PAL_DrawNumber(OrigPlayerRoles.rgwDexterity[w] + PAL_GetPlayerDexterity(w) -
            gpGlobals->g.PlayerRoles.rgwDexterity[w],
            4, PAL_XY(133, 155), kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(PAL_GetPlayerDexterity(w), 4, PAL_XY(195, 155),
            kNumColorYellow, kNumAlignRight);

         PAL_DrawNumber(OrigPlayerRoles.rgwFleeRate[w] + PAL_GetPlayerFleeRate(w) -
            gpGlobals->g.PlayerRoles.rgwFleeRate[w],
            4, PAL_XY(133, 173), kNumColorYellow, kNumAlignRight);
         PAL_DrawNumber(PAL_GetPlayerFleeRate(w), 4, PAL_XY(195, 173),
            kNumColorYellow, kNumAlignRight);

         //
         // Update the screen and wait for key
         //
         VIDEO_UpdateScreen(&rect1);
         PAL_WaitForKey(3000);

         OrigPlayerRoles = gpGlobals->g.PlayerRoles;
      }

      //
      // Increasing of other hidden levels
      //
      iTotalCount = 0;

      iTotalCount += gpGlobals->Exp.rgAttackExp[w].wCount;
      iTotalCount += gpGlobals->Exp.rgDefenseExp[w].wCount;
      iTotalCount += gpGlobals->Exp.rgDexterityExp[w].wCount;
      iTotalCount += gpGlobals->Exp.rgFleeExp[w].wCount;
      iTotalCount += gpGlobals->Exp.rgHealthExp[w].wCount;
      iTotalCount += gpGlobals->Exp.rgMagicExp[w].wCount;
      iTotalCount += gpGlobals->Exp.rgMagicPowerExp[w].wCount;

      if (iTotalCount > 0)
      {
#define CHECK_HIDDEN_EXP(expname, statname, label)          \
{                                                           \
   dwExp = g_Battle.iExpGained;                             \
   dwExp *= gpGlobals->Exp.expname[w].wCount;               \
   dwExp /= iTotalCount;                                    \
   dwExp *= 2;                                              \
                                                            \
   dwExp += gpGlobals->Exp.expname[w].wExp;                 \
                                                            \
   if (gpGlobals->Exp.expname[w].wLevel > MAX_LEVELS)       \
   {                                                        \
      gpGlobals->Exp.expname[w].wLevel = MAX_LEVELS;        \
   }                                                        \
                                                            \
   while (dwExp >= gpGlobals->g.rgLevelUpExp[gpGlobals->Exp.expname[w].wLevel]) \
   {                                                        \
      dwExp -= gpGlobals->g.rgLevelUpExp[gpGlobals->Exp.expname[w].wLevel]; \
      gpGlobals->g.PlayerRoles.statname[w] += RandomLong(1, 2); \
      if (gpGlobals->Exp.expname[w].wLevel < MAX_LEVELS)    \
      {                                                     \
         gpGlobals->Exp.expname[w].wLevel++;                \
      }                                                     \
   }                                                        \
                                                            \
   gpGlobals->Exp.expname[w].wExp = (WORD)dwExp;            \
                                                            \
   if (gpGlobals->g.PlayerRoles.statname[w] !=              \
      OrigPlayerRoles.statname[w])                          \
   {                                                        \
      PAL_CreateSingleLineBox(PAL_XY(83, 60), 8, FALSE);    \
      PAL_DrawText(PAL_GetWord(gpGlobals->g.PlayerRoles.rgwName[w]), PAL_XY(95, 70), \
         0, FALSE, FALSE);                                  \
      PAL_DrawText(PAL_GetWord(label), PAL_XY(143, 70),     \
         0, FALSE, FALSE);                                  \
      PAL_DrawText(PAL_GetWord(BATTLEWIN_LEVELUP_LABEL), PAL_XY(175, 70),  \
         0, FALSE, FALSE);                                  \
      PAL_DrawNumber(gpGlobals->g.PlayerRoles.statname[w] - \
         OrigPlayerRoles.statname[w],                       \
         5, PAL_XY(188, 74), kNumColorYellow, kNumAlignRight); \
      VIDEO_UpdateScreen(&rect);                            \
      PAL_WaitForKey(3000);                                 \
   }                                                        \
}

         CHECK_HIDDEN_EXP(rgHealthExp, rgwMaxHP, STATUS_LABEL_HP);
         CHECK_HIDDEN_EXP(rgMagicExp, rgwMaxMP, STATUS_LABEL_MP);
         CHECK_HIDDEN_EXP(rgAttackExp, rgwAttackStrength, STATUS_LABEL_ATTACKPOWER);
         CHECK_HIDDEN_EXP(rgMagicPowerExp, rgwMagicStrength, STATUS_LABEL_MAGICPOWER);
         CHECK_HIDDEN_EXP(rgDefenseExp, rgwDefense, STATUS_LABEL_RESISTANCE);
         CHECK_HIDDEN_EXP(rgDexterityExp, rgwDexterity, STATUS_LABEL_DEXTERITY);
         CHECK_HIDDEN_EXP(rgFleeExp, rgwFleeRate, STATUS_LABEL_FLEERATE);

#undef CHECK_HIDDEN_EXP
      }

      //
      // Learn all magics at the current level
      //
      j = 0;

      while (j < gpGlobals->g.nLevelUpMagic)
      {
         if (gpGlobals->g.lprgLevelUpMagic[j].m[w].wMagic == 0 ||
            gpGlobals->g.lprgLevelUpMagic[j].m[w].wLevel > gpGlobals->g.PlayerRoles.rgwLevel[w])
         {
            j++;
            continue;
         }

         if (PAL_AddMagic(w, gpGlobals->g.lprgLevelUpMagic[j].m[w].wMagic))
         {
            PAL_CreateSingleLineBox(PAL_XY(65, 105), 10, FALSE);

            PAL_DrawText(PAL_GetWord(gpGlobals->g.PlayerRoles.rgwName[w]),
               PAL_XY(75, 115), 0, FALSE, FALSE);
            PAL_DrawText(PAL_GetWord(BATTLEWIN_ADDMAGIC_LABEL), PAL_XY(75 + 16 * 3, 115),
               0, FALSE, FALSE);
            PAL_DrawText(PAL_GetWord(gpGlobals->g.lprgLevelUpMagic[j].m[w].wMagic),
               PAL_XY(75 + 16 * 5, 115), 0x1B, FALSE, FALSE);

            VIDEO_UpdateScreen(&rect);
            PAL_WaitForKey(3000);
         }

         j++;
      }
   }

   //
   // Run the post-battle scripts
   //
   for (i = 0; i <= g_Battle.wMaxEnemyIndex; i++)
   {
      PAL_RunTriggerScript(g_Battle.rgEnemy[i].wScriptOnBattleEnd, i);
   }

   //
   // Recover automatically after each battle
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      w = gpGlobals->rgParty[i].wPlayerRole;

#if 1//def PAL_CLASSIC
      gpGlobals->g.PlayerRoles.rgwHP[w] +=
         (gpGlobals->g.PlayerRoles.rgwMaxHP[w] - gpGlobals->g.PlayerRoles.rgwHP[w]) / 2;
      gpGlobals->g.PlayerRoles.rgwMP[w] +=
         (gpGlobals->g.PlayerRoles.rgwMaxMP[w] - gpGlobals->g.PlayerRoles.rgwMP[w]) / 2;
#else
      if (gpGlobals->g.PlayerRoles.rgwHP[w] == 0)
      {
         gpGlobals->g.PlayerRoles.rgwHP[w] = 1;
      }
      else if (g_Battle.iExpGained > 0)
      {
         FLOAT f =
            (gpGlobals->g.rgLevelUpExp[gpGlobals->g.PlayerRoles.rgwLevel[w]] / 5.0f) / g_Battle.iExpGained;

         if (f < 2)
         {
            f = 2;
         }

         gpGlobals->g.PlayerRoles.rgwHP[w] +=
            (gpGlobals->g.PlayerRoles.rgwMaxHP[w] - gpGlobals->g.PlayerRoles.rgwHP[w]) / f;
         gpGlobals->g.PlayerRoles.rgwMP[w] +=
            (gpGlobals->g.PlayerRoles.rgwMaxMP[w] - gpGlobals->g.PlayerRoles.rgwMP[w]) / f / 1.2;
      }
#endif
   }
}

VOID
PAL_BattleEnemyEscape(
   VOID
)
/*++
  Purpose:

    Enemy flee the battle.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   int j, x, y, w;
   BOOL f = TRUE;

   SOUND_Play(45);

   //
   // Show the animation
   //
   while (f)
   {
   	  f = FALSE;

   	  for (j = 0; j <= g_Battle.wMaxEnemyIndex; j++)
   	  {
   	  	 if (g_Battle.rgEnemy[j].wObjectID == 0)
   	  	 {
   	  	 	continue;
   	  	 }

   	  	 x = PAL_X(g_Battle.rgEnemy[j].pos) - 5;
   	  	 y = PAL_Y(g_Battle.rgEnemy[j].pos);

   	  	 g_Battle.rgEnemy[j].pos = PAL_XY(x, y);

   	  	 w = PAL_RLEGetWidth(PAL_SpriteGetFrame(g_Battle.rgEnemy[j].lpSprite, 0));

   	  	 if (x + w > 0)
   	  	 {
   	  	 	f = TRUE;
   	  	 }
   	  }

   	  PAL_BattleMakeScene();
      SDL_BlitSurface(g_Battle.lpSceneBuf, NULL, gpScreen, NULL);
      VIDEO_UpdateScreen(NULL);

      UTIL_Delay(10);
   }

   UTIL_Delay(500);
   g_Battle.BattleResult = kBattleResultTerminated;
}

VOID
PAL_BattlePlayerEscape(
   VOID
)
/*++
  Purpose:

    Player flee the battle.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   int         i, j;
   WORD        wPlayerRole;

   SOUND_Play(45);

   PAL_BattleUpdateFighters();

   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      wPlayerRole = gpGlobals->rgParty[i].wPlayerRole;

      if (gpGlobals->g.PlayerRoles.rgwHP[wPlayerRole] > 0)
      {
         g_Battle.rgPlayer[i].wCurrentFrame = 0;
      }
   }

   for (i = 0; i < 16; i++)
   {
      for (j = 0; j <= gpGlobals->wMaxPartyMemberIndex; j++)
      {
         wPlayerRole = gpGlobals->rgParty[j].wPlayerRole;

         if (gpGlobals->g.PlayerRoles.rgwHP[wPlayerRole] > 0)
         {
            //
            // TODO: This is still not the same as the original game
            //
            switch (j)
            {
            case 0:
               if (gpGlobals->wMaxPartyMemberIndex > 0)
               {
                  g_Battle.rgPlayer[j].pos =
                     PAL_XY(PAL_X(g_Battle.rgPlayer[j].pos) + 4,
                            PAL_Y(g_Battle.rgPlayer[j].pos) + 6);
                  break;
               }

            case 1:
               g_Battle.rgPlayer[j].pos =
                  PAL_XY(PAL_X(g_Battle.rgPlayer[j].pos) + 4,
                         PAL_Y(g_Battle.rgPlayer[j].pos) + 4);
               break;

            case 2:
               g_Battle.rgPlayer[j].pos =
                  PAL_XY(PAL_X(g_Battle.rgPlayer[j].pos) + 6,
                         PAL_Y(g_Battle.rgPlayer[j].pos) + 3);
               break;

            default:
               assert(FALSE); // Not possible
               break;
            }
         }
      }

      PAL_BattleDelay(1, 0, FALSE);
   }

   //
   // Remove all players from the screen
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      g_Battle.rgPlayer[i].pos = PAL_XY(9999, 9999);
   }

   PAL_BattleDelay(1, 0, FALSE);

   g_Battle.BattleResult = kBattleResultFleed;
}

BATTLERESULT
PAL_StartBattle(
   WORD        wEnemyTeam,
   BOOL        fIsBoss
)
/*++
  Purpose:

    Start a battle.

  Parameters:

    [IN]  wEnemyTeam - the number of the enemy team.

    [IN]  fIsBoss - TRUE for boss fight (not allowed to flee).

  Return value:

    The result of the battle.

--*/
{
   int            i;
   WORD           w, wPrevWaveLevel;
   SHORT          sPrevWaveProgression;

   //
   // Set the screen waving effects
   //
   wPrevWaveLevel = gpGlobals->wScreenWave;
   sPrevWaveProgression = gpGlobals->sWaveProgression;

   gpGlobals->sWaveProgression = 0;
   gpGlobals->wScreenWave = gpGlobals->g.lprgBattleField[gpGlobals->wNumBattleField].wScreenWave;

   //
   // Make sure everyone in the party is alive, also clear all hidden
   // EXP count records
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      w = gpGlobals->rgParty[i].wPlayerRole;

      if (gpGlobals->g.PlayerRoles.rgwHP[w] == 0)
      {
         gpGlobals->g.PlayerRoles.rgwHP[w] = 1;
         gpGlobals->rgPlayerStatus[w][kStatusPuppet] = 0;
      }

      gpGlobals->Exp.rgHealthExp[w].wCount = 0;
      gpGlobals->Exp.rgMagicExp[w].wCount = 0;
      gpGlobals->Exp.rgAttackExp[w].wCount = 0;
      gpGlobals->Exp.rgMagicPowerExp[w].wCount = 0;
      gpGlobals->Exp.rgDefenseExp[w].wCount = 0;
      gpGlobals->Exp.rgDexterityExp[w].wCount = 0;
      gpGlobals->Exp.rgFleeExp[w].wCount = 0;
   }

   //
   // Clear all item-using records
   //
   for (i = 0; i < MAX_INVENTORY; i++)
   {
      gpGlobals->rgInventory[i].nAmountInUse = 0;
   }

   //
   // Store all enemies
   //
   for (i = 0; i < MAX_ENEMIES_IN_TEAM; i++)
   {
      memset(&(g_Battle.rgEnemy[i]), 0, sizeof(BATTLEENEMY));
      w = gpGlobals->g.lprgEnemyTeam[wEnemyTeam].rgwEnemy[i];

      if (w == 0xFFFF)
      {
         break;
      }

      if (w != 0)
      {
         g_Battle.rgEnemy[i].e = gpGlobals->g.lprgEnemy[gpGlobals->g.rgObject[w].enemy.wEnemyID];
         g_Battle.rgEnemy[i].wObjectID = w;
         g_Battle.rgEnemy[i].state = kFighterWait;
         g_Battle.rgEnemy[i].wScriptOnTurnStart = gpGlobals->g.rgObject[w].enemy.wScriptOnTurnStart;
         g_Battle.rgEnemy[i].wScriptOnBattleEnd = gpGlobals->g.rgObject[w].enemy.wScriptOnBattleEnd;
         g_Battle.rgEnemy[i].wScriptOnReady = gpGlobals->g.rgObject[w].enemy.wScriptOnReady;
         g_Battle.rgEnemy[i].iColorShift = 0;

#ifndef PAL_CLASSIC
         g_Battle.rgEnemy[i].flTimeMeter = int2F(50);

         //
         // HACK: Otherwise the black thief lady will be too hard to beat
         //
         if (g_Battle.rgEnemy[i].e.wDexterity == 164)
         {
            g_Battle.rgEnemy[i].e.wDexterity /= ((gpGlobals->wMaxPartyMemberIndex == 0) ? 6 : 3);
         }

         //
         // HACK: Heal up automatically for final boss
         //
         if (g_Battle.rgEnemy[i].e.wHealth == 32760)
         {
            for (w = 0; w < MAX_PLAYER_ROLES; w++)
            {
               gpGlobals->g.PlayerRoles.rgwHP[w] = gpGlobals->g.PlayerRoles.rgwMaxHP[w];
               gpGlobals->g.PlayerRoles.rgwMP[w] = gpGlobals->g.PlayerRoles.rgwMaxMP[w];
            }
         }

         //
         // Yet another HACKs
         //
         if ((SHORT)g_Battle.rgEnemy[i].e.wDexterity == -32)
         {
            g_Battle.rgEnemy[i].e.wDexterity = 0; // for Grandma Knife
         }
         else if (g_Battle.rgEnemy[i].e.wDexterity == 20)
         {
            //
            // for Fox Demon
            //
            if (gpGlobals->g.PlayerRoles.rgwLevel[0] < 15)
            {
               g_Battle.rgEnemy[i].e.wDexterity = 8;
            }
            else if (gpGlobals->g.PlayerRoles.rgwLevel[4] > 28 ||
               gpGlobals->Exp.rgPrimaryExp[4].wExp > 0)
            {
               g_Battle.rgEnemy[i].e.wDexterity = 60;
            }
         }
         else if (g_Battle.rgEnemy[i].e.wExp == 250 &&
            g_Battle.rgEnemy[i].e.wCash == 1100)
         {
            g_Battle.rgEnemy[i].e.wDexterity += 12; // for Snake Demon
         }
         else if ((SHORT)g_Battle.rgEnemy[i].e.wDexterity == -60)
         {
            g_Battle.rgEnemy[i].e.wDexterity = 15; // for Spider
         }
         else if ((SHORT)g_Battle.rgEnemy[i].e.wDexterity == -30)
         {
            g_Battle.rgEnemy[i].e.wDexterity = (WORD)-10; // for Stone Head
         }
         else if ((SHORT)g_Battle.rgEnemy[i].e.wDexterity == -16)
         {
            g_Battle.rgEnemy[i].e.wDexterity = 0; // for Zombie
         }
         else if ((SHORT)g_Battle.rgEnemy[i].e.wDexterity == -20)
         {
            g_Battle.rgEnemy[i].e.wDexterity = -8; // for Flower Demon
         }
         else if (g_Battle.rgEnemy[i].e.wLevel < 20 &&
            gpGlobals->wNumScene >= 0xD8 && gpGlobals->wNumScene <= 0xE2)
         {
            //
            // for low-level monsters in the Cave of Trial
            //
            g_Battle.rgEnemy[i].e.wLevel += 15;
            g_Battle.rgEnemy[i].e.wDexterity += 25;
         }
         else if (gpGlobals->wNumScene == 0x90)
         {
            g_Battle.rgEnemy[i].e.wDexterity += 25; // for Tower Dragons
         }
         else if (g_Battle.rgEnemy[i].e.wLevel == 2 &&
            g_Battle.rgEnemy[i].e.wCash == 48)
         {
            g_Battle.rgEnemy[i].e.wDexterity += 8; // for Miao Fists
         }
         else if (g_Battle.rgEnemy[i].e.wLevel == 4 &&
            g_Battle.rgEnemy[i].e.wCash == 240)
         {
            g_Battle.rgEnemy[i].e.wDexterity += 18; // for Fat Miao
         }
         else if (g_Battle.rgEnemy[i].e.wLevel == 16 &&
            g_Battle.rgEnemy[i].e.wMagicRate == 4 &&
            g_Battle.rgEnemy[i].e.wAttackEquivItemRate == 4)
         {
            g_Battle.rgEnemy[i].e.wDexterity += 50; // for Black Spider
         }
#endif
      }
   }

   g_Battle.wMaxEnemyIndex = i - 1;

   //
   // Store all players
   //
   for (i = 0; i <= gpGlobals->wMaxPartyMemberIndex; i++)
   {
      g_Battle.rgPlayer[i].flTimeMeter = int2F(15);
#ifndef PAL_CLASSIC
      g_Battle.rgPlayer[i].flTimeSpeedModifier = int2F(2);
      g_Battle.rgPlayer[i].sTurnOrder = -1;
#endif
      g_Battle.rgPlayer[i].wHidingTime = 0;
      g_Battle.rgPlayer[i].state = kFighterWait;
      g_Battle.rgPlayer[i].action.sTarget = -1;
      g_Battle.rgPlayer[i].fDefending = FALSE;
      g_Battle.rgPlayer[i].wCurrentFrame = 0;
      g_Battle.rgPlayer[i].iColorShift = FALSE;
   }

   //
   // Load sprites and background
   //
   PAL_LoadBattleSprites();
   PAL_LoadBattleBackground();

   //
   // Create the surface for scene buffer
   //
   g_Battle.lpSceneBuf =
      SDL_CreateRGBSurface(gpScreen->flags & ~SDL_HWSURFACE, 320, 200, 8,
      gpScreen->format->Rmask, gpScreen->format->Gmask,
      gpScreen->format->Bmask, gpScreen->format->Amask);

   if (g_Battle.lpSceneBuf == NULL)
   {
      TerminateOnError("PAL_StartBattle(): creating surface for scene buffer failed!");
   }

#if SDL_VERSION_ATLEAST(2, 0, 0)
   SDL_SetSurfacePalette(g_Battle.lpSceneBuf, gpScreen->format->palette);
#else
   SDL_SetPalette(g_Battle.lpSceneBuf, SDL_PHYSPAL | SDL_LOGPAL, VIDEO_GetPalette(), 0, 256);
#endif

   PAL_UpdateEquipments();

   g_Battle.iExpGained = 0;
   g_Battle.iCashGained = 0;

   g_Battle.fIsBoss = fIsBoss;
   g_Battle.fEnemyCleared = FALSE;
   g_Battle.fEnemyMoving = FALSE;
   g_Battle.iHidingTime = 0;
   g_Battle.wMovingPlayerIndex = 0;

   g_Battle.UI.szMsg[0] = '\0';
   g_Battle.UI.szNextMsg[0] = '\0';
   g_Battle.UI.dwMsgShowTime = 0;
   g_Battle.UI.state = kBattleUIWait;
   g_Battle.UI.fAutoAttack = FALSE;
   g_Battle.UI.wSelectedIndex = 0;
   g_Battle.UI.wPrevEnemyTarget = 0;

   memset(g_Battle.UI.rgShowNum, 0, sizeof(g_Battle.UI.rgShowNum));

   g_Battle.lpSummonSprite = NULL;
   g_Battle.sBackgroundColorShift = 0;

   gpGlobals->fInBattle = TRUE;
   g_Battle.BattleResult = kBattleResultPreBattle;

   PAL_BattleUpdateFighters();

   //
   // Load the battle effect sprite.
   //
   i = PAL_MKFGetChunkSize(10, gpGlobals->f.fpDATA);
   g_Battle.lpEffectSprite = UTIL_malloc(i);

   PAL_MKFReadChunk(g_Battle.lpEffectSprite, i, 10, gpGlobals->f.fpDATA);

#ifdef PAL_CLASSIC
   g_Battle.Phase = kBattlePhaseSelectAction;
   g_Battle.fRepeat = FALSE;
   g_Battle.fForce = FALSE;
   g_Battle.fFlee = FALSE;
#endif

#ifdef PAL_ALLOW_KEYREPEAT
   SDL_EnableKeyRepeat(120, 75);
#endif

   //
   // Run the main battle routine.
   //
   i = PAL_BattleMain();

#ifdef PAL_ALLOW_KEYREPEAT
   SDL_EnableKeyRepeat(0, 0);
   PAL_ClearKeyState();
   g_InputState.prevdir = kDirUnknown;
#endif

   if (i == kBattleResultWon)
   {
      //
      // Player won the battle. Add the Experience points.
      //
      PAL_BattleWon();
   }

   //
   // Clear all item-using records
   //
   for (w = 0; w < MAX_INVENTORY; w++)
   {
      gpGlobals->rgInventory[w].nAmountInUse = 0;
   }

   //
   // Clear all player status, poisons and temporary effects
   //
   PAL_ClearAllPlayerStatus();
   for (w = 0; w < MAX_PLAYER_ROLES; w++)
   {
      PAL_CurePoisonByLevel(w, 3);
      PAL_RemoveEquipmentEffect(w, kBodyPartExtra);
   }

   //
   // Free all the battle sprites
   //
   PAL_FreeBattleSprites();
   free(g_Battle.lpEffectSprite);

   //
   // Free the surfaces for the background picture and scene buffer
   //
   SDL_FreeSurface(g_Battle.lpBackground);
   SDL_FreeSurface(g_Battle.lpSceneBuf);

   g_Battle.lpBackground = NULL;
   g_Battle.lpSceneBuf = NULL;

   gpGlobals->fInBattle = FALSE;

   PAL_PlayMUS(gpGlobals->wNumMusic, TRUE, 1);

   //
   // Restore the screen waving effects
   //
   gpGlobals->sWaveProgression = sPrevWaveProgression;
   gpGlobals->wScreenWave = wPrevWaveLevel;

   return i;
}
