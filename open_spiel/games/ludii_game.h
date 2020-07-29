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
// NOTE: we currently support up to 9 options being passed into Ludii
// I don't think there are currently any Ludii games with anywhere close
// to that many option categories, so this should suffice for a long time.
//
//
// // Parameters:
//       "game_name"   string  Name of the game (in Ludii) to load
//                             (default "Tic-Tac-Toe.lud")
//       "lud_opt_1"   string  First option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_2"   string  Second option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_3"   string  Third option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_4"   string  Fourth option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_5"   string  Fifth option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_6"   string  Sixth option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_7"   string  Seventh option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_8"   string  Eighth option to pass to Ludii if not equal to
//                             empty string (default "")
//       "lud_opt_9"   string  Ninth option to pass to Ludii if not equal to
//                             empty string (default "")
//
//
// Note: implementation partially based on
// https://github.com/facebookincubator/Polygames/blob/master/games/ludii/ludii_game_wrapper.h

namespace open_spiel {
namespace ludii_game {


// State of an in-play game.
class LudiiState : public State {
 public:
  LudiiState(std::shared_ptr<const Game> game);

  LudiiState(const LudiiState&) = default;
  LudiiState& operator=(const LudiiState&) = default;

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
  //--------------------------
  // Regular OpenSpiel fields
  //--------------------------
  Player current_player_ = 0;         // Player zero goes first
  Player outcome_ = kInvalidPlayer;

  //--------------------------
  // Ludii / Java fields
  //--------------------------

  /** Pointer to the JNI environment, allows for communication with Ludii's Java code */
  JNIEnv* jenv;
  /** Our Game wrapper */
  LudiiGameWrapper ludiiGameWrapper;
  /** Our object of Java's LudiiStateWrapper type */
  jobject ludiiStateWrapperJavaObject;
  /** Method ID for the returns() method in Java */
  jmethodID returnsMethodID;
  /** Method ID for the isTerminal() method in Java */
  jmethodID isTerminalMethodID;
  /** Method ID for the toTensor() method in Java */
  jmethodID toTensorMethodID;
  /** Method ID for the currentPlayer() method in Java */
  jmethodID currentPlayerMethodID;
  /** Method ID for the reset() method in Java */
  jmethodID resetMethodID;
};

// Game object.
class LudiiGame : public Game {
 public:
  explicit LudiiGame(const GameParameters& params);

  int NumDistinctActions() const override;
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

  // Our object of Java's LudiiGameWrapper type. No need to make deep copies of this in Java
  jobject ludiiGameWrapperJavaObject;

 private:
  // Pointer to the JNI environment, allows for communication with Ludii's Java code
  JNIEnv* jenv;

  // Method IDs for various Java methods
  jmethodID numPlayersMethodID;
  jmethodID stateTensorsShapeMethodID;

  // Shape for state tensors.
  // This remains constant throughout episodes, so can just compute it once and store
  const int num_state_channels;
  const int num_state_cols;
  const int num_state_rows;
};

}  // namespace ludii_game
}  // namespace open_spiel

#endif  // OPEN_SPIEL_GAMES_LUDII_GAME_H_
