/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef Scenario_h__
#define Scenario_h__

#include "CriteriaHandler.h"
#include <unordered_set>

class InstanceScenario;

struct ScenarioData;
struct ScenarioEntry;
struct ScenarioStepEntry;

namespace WorldPackets
{
    namespace Achievement
    {
        struct CriteriaProgress;
    }

    namespace Scenario
    {
        struct BonusObjectiveData;
        class ScenarioState;
    }
}

enum ScenarioStepState
{
    SCENARIO_STEP_INVALID       = 0,
    SCENARIO_STEP_NOT_STARTED   = 1,
    SCENARIO_STEP_IN_PROGRESS   = 2,
    SCENARIO_STEP_DONE          = 3
};

enum ScenarioInstanceType
{
    SCENARIO_INSTANCE_TYPE_SCENARIO          = 1,
    SCENARIO_INSTANCE_TYPE_INSTANCE_SCENARIO = 2,
};

class TC_GAME_API Scenario : public CriteriaHandler
{
    public:
        Scenario(ScenarioData const* scenarioData);
        ~Scenario();

        void Reset() override;
        void SetStep(ScenarioStepEntry const* step);

        virtual void CompleteStep(ScenarioStepEntry const* step);
        virtual void CompleteScenario();

        virtual void OnPlayerEnter(Player* player);
        virtual void OnPlayerExit(Player* player);
        virtual void Update(uint32 /*diff*/) { }

        bool IsComplete();
        void SetStepState(ScenarioStepEntry const* step, ScenarioStepState state) { _stepStates[step] = state; }
        ScenarioEntry const* GetEntry() const;
        ScenarioStepState GetStepState(ScenarioStepEntry const* step);
        ScenarioStepEntry const* GetStep() const { return _currentstep; }
        ScenarioStepEntry const* GetFirstStep() const;
        void CompleteCurrStep();

        void SendScenarioState(Player* player);
        void SendBootPlayer(Player* player);

        void SendScenarioEvent(Player* player, uint32 eventId);

        inline bool IsInstanceScenarioo() const { return _scenarioType == SCENARIO_INSTANCE_TYPE_INSTANCE_SCENARIO; }
        InstanceScenario* ToInstanceScenario() { if (IsInstanceScenarioo()) return reinterpret_cast<InstanceScenario*>(this); else return nullptr; }
        InstanceScenario const* ToInstanceScenario() const { if (IsInstanceScenarioo()) return reinterpret_cast<InstanceScenario const*>(this); else return nullptr; }

    protected:
        GuidUnorderedSet _players;

        void SendCriteriaUpdate(Criteria const* criteria, CriteriaProgress const* progress, Seconds timeElapsed, bool timedCompleted) const override;
        void SendCriteriaProgressRemoved(uint32 /*criteriaId*/) override { }

        bool CanUpdateCriteriaTree(Criteria const* criteria, CriteriaTree const* tree, Player* referencePlayer) const override;
        bool CanCompleteCriteriaTree(CriteriaTree const* tree) override;
        void CompletedCriteriaTree(CriteriaTree const* tree, Player* referencePlayer) override;
        void AfterCriteriaTreeUpdate(CriteriaTree const* /*tree*/, Player* /*referencePlayer*/) override { }

        void SendPacket(WorldPacket const* data) const override;

        void SendAllData(Player const* /*receiver*/) const override { }

        void BuildScenarioState(WorldPackets::Scenario::ScenarioState* scenarioState);

        std::vector<WorldPackets::Scenario::BonusObjectiveData> GetBonusObjectivesData();
        std::vector<WorldPackets::Achievement::CriteriaProgress> GetCriteriasProgress();

        CriteriaList const& GetCriteriaByType(CriteriaTypes type, uint32 asset) const override;
        ScenarioData const* _data;
        Ashamane::AnyData Variables;

        ScenarioInstanceType _scenarioType;

    private:
        ScenarioStepEntry const* _currentstep;
        std::map<ScenarioStepEntry const*, ScenarioStepState> _stepStates;
};

#endif // Scenario_h__
