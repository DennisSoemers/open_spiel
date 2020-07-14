// Copyright 2019 DeepMind Technologies Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_SPIEL_GAMES_LUDII_GAME_H_
#define OPEN_SPIEL_GAMES_LUDII_GAME_H_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"

// Wrapper that can represent any arbitrary game
// loaded from the Ludii General Game System:
// https://ludii.games/
//
// Parameters: TODO

namespace open_spiel {
namespace ludii_game {


// State of an in-play game.
class LudiiState : public State {
 public:
  LudiiState(std::shared_ptr<const Game> game);

  LudiiState(const TicTacToeState&) = default;
  LudiiState& operator=(const TicTacToeState&) = default;

  Player CurrentPlayer() const override {
    return IsTerminal() ? kTerminalPlayerId : current_player_;
  }
  std::string ActionToString(Player player, Action action_id) const override;
  std::string ToString() const override;
  bool IsTerminal() const override;
  std::vector<double> Returns() const override;
  std::string InformationStateString(Player player) const override;
  std::string ObservationString(Player player) const override;
  void ObservationTensor(Player player,
                         std::vector<double>* values) const override;
  std::unique_ptr<State> Clone() const override;
  void UndoAction(Player player, Action move) override;
  std::vector<Action> LegalActions() const override;

 protected:
  void DoApplyAction(Action move) override;

 private:
  Player current_player_ = 0;         // Player zero goes first
  Player outcome_ = kInvalidPlayer;
};

// Game object.
class LudiiGame : public Game {
 public:
  explicit LudiiGame(const GameParameters& params);
  int NumDistinctActions() const override { return kNumCells; }
  std::unique_ptr<State> NewInitialState() const override {
    return std::unique_ptr<State>(new LudiiState(shared_from_this()));
  }
  int NumPlayers() const override { return kNumPlayers; }
  double MinUtility() const override { return -1; }
  double UtilitySum() const override { return 0; }
  double MaxUtility() const override { return 1; }
  std::shared_ptr<const Game> Clone() const override {
    return std::shared_ptr<const Game>(new LudiiGame(*this));
  }
  std::vector<int> ObservationTensorShape() const override {
    return {kCellStates, kNumRows, kNumCols};
  }
  int MaxGameLength() const override { return kNumCells; }
};

CellState PlayerToState(Player player);
std::string StateToString(CellState state);

inline std::ostream& operator<<(std::ostream& stream, const CellState& state) {
  return stream << StateToString(state);
}

}  // namespace ludii_game
}  // namespace open_spiel

#endif  // OPEN_SPIEL_GAMES_LUDII_GAME_H_
