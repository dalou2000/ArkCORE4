/*
* Copyright (C) 2012-2013 HTCore <http://cata.vfire-core.com/>
* Copyright (C) 2012-2013 WoW Source <http://wow.amgi-it.ro/>
* by Shee Shen
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Spell.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"

#define spell_soleil 73872 // Coup de soleil 
#define spell_invocation 80352 // Invocation d'un orbe du soleil
#define spell_enfer 87653 // Bond du feu d'enfer
#define spell_vent 89130 // Vents solaires
#define spell_feu 89131 // Feu solaire
#define spell_bene 76355 // Bénédiction du soleil

#define SPELL_SOLAR 74108

enum Texts
{
    SAY_AGGRO = 0,
    SAY_EVENT = 1,
    SAY_SLAY = 2,
    SAY_DEATH = 3,
};

/*******
** Rajh
********/
class boss_rajh: public CreatureScript
{
public: 
 boss_rajh() : CreatureScript("boss_rajh") { } 

 struct boss_rajhAI : public ScriptedAI
    {
        boss_rajhAI(Creature *c) : ScriptedAI(c) {}

		uint32 soleil;
        uint32 invocation;
		uint32 enfer;
		uint32 vent;
		uint32 feu;
		uint32 bene;
		uint32 slay;
		
        void Reset()
        {
			soleil = 5000;
			invocation = 10000; 
			enfer = 3000;
			vent = 60000;
			feu = 15000;
			bene = 4000;	
			slay = 9000;		   
        }

        void EnterCombat(Unit* /*who*/) 
        {
			Talk(SAY_AGGRO);
        }

        void JustDied(Unit* /*killer*/)
        {
			Talk(SAY_DEATH);
			
        }

        void UpdateAI(uint32 diff)
        {
                if (!UpdateVictim())
                    return;

            if (soleil<= diff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, spell_soleil);
                soleil   = 5000; 
            } else soleil -= diff; 
				
            if (invocation<= diff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, spell_invocation);
                invocation   = 10000; 
            } else invocation -= diff; 
				
            if (enfer<= diff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, spell_enfer);
                enfer   = 3000; 
            } else enfer -= diff; 
				
            if (vent<= diff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, spell_vent);
                vent   = 60000; 
            } else vent -= diff; 
				
            if (feu<= diff)
            {
				Talk(SAY_EVENT);
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, spell_feu);
                feu   = 15000; 
            } else feu -= diff; 
				
            if (bene<= diff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, spell_bene);
                bene   = 4000; 
            } else bene -= diff;
			
			if (slay<= diff)
            {
                Talk(SAY_SLAY);
				slay   = 9000; 
            } else slay -= diff;
	
			DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_rajhAI (pCreature);
    }

};

/***************
** vent Solaire
***************/
class npc_solar_wind : public CreatureScript
{
public:
    npc_solar_wind() : CreatureScript("npc_solar_wind") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_solar_windAI(pCreature);
    }

    struct npc_solar_windAI : public ScriptedAI
    {
        npc_solar_windAI(Creature* c) : ScriptedAI(c)
        {
			
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->setFaction(16); // Same faction as bosses
			if (IsHeroic())
			{
				DoCast(SPELL_SOLAR);
			}

            me->DespawnOrUnsummon(60000);
        }
    };
};

void AddSC_boss_rajh()
{
    new boss_rajh();
	new npc_solar_wind();
}