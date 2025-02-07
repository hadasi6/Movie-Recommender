#include "UsersLoader.h"
#include "RecommendationSystemLoader.h"
#include "RecommendationSystem.h"f
#include "Movie.h"
#include <iostream>
#include <string>
#define EXIT_SUCCESS_TEST 0
#define EXIT_FAIL_TEST 2
static std::string BASE_PATH = "ex5/io/";
#define THRESHOLD 0.01

int main ()
{
  int status = 0;

  try
    {
      // check all function for RS exist
      RecommendationSystem rec;
      sp_movie a = rec.add_movie_to_rs ("Batman", 2022, {1, 2, 3, 4});
      sp_movie b = rec.add_movie_to_rs ("StarWars", 1977, {1, 2, 3, 5});
      sp_movie c = rec.add_movie_to_rs ("ForestGump", 1994, {1, 2, 3, 4});

      // check all functions for user and UsersLoader exist and movie
      auto rs1 = RecommendationSystemLoader::create_rs_from_movies (
          BASE_PATH + "presubmit.in_m");
      std::vector<User> users = UsersLoader::create_users (
          BASE_PATH + "presubmit.in_u", std::move (rs1));
      sp_movie movie = users[0].get_rs_recommendation_by_content ();
      double s = users[0].get_rs_prediction_score_for_movie ("Titanic", 1997, 2);
      sp_movie m2 = users[0].get_rs_recommendation_by_cf (2);
      if (s < 0)
        {
          std::cerr << "a score should be larger or equal to 0" << std::endl;
          status = 1;
        }

      // check movie functions and some results
      if (movie->get_name () != "Batman" || movie->get_year () != 2022)
        {
          status = 1;
          std::cerr
              << "Recommend by content with username=Sofia failed! Received="
              << *movie << " Expected=Batman (2022)" << std::endl;
        }
      double y = users[2].get_rs_prediction_score_for_movie ("Twilight", 2008, 2);
      if (std::abs (y - 3.5244) > THRESHOLD)
        {
          status = 1;
          std::cerr
              << "Predict movie score for moviename=Twilight , username=Nicole, k=2 failed! Received="
              << y << " Expected=3.5244" << std::endl;
        }
      double z = users[2].get_rs_prediction_score_for_movie ("Titanic", 1997, 2);
      if (std::abs (z - 5.46432) > THRESHOLD)
        {
          status = 1;
          std::cerr
              << "Predict movie score for moviename=Titanic, username=Nicole, k=2 failed! Received="
              << z << " Expected=5.46432" << std::endl;
        }
      if (status != 0)
        {
          std::cerr << "Failed presubmit tests, you have errors" << std::endl;
          return 1;
        }
    }
  catch (const std::exception &e)
    {
      std::cerr
          << "Your program crashed by arised exception - presubmit tests failed"
          << std::endl;
      return 2;
    }

  return 0;
}
