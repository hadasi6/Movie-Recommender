#include <algorithm>
#include "cmath"
#include "RecommendationSystem.h"

sp_movie RecommendationSystem::add_movie_to_rs (const std::string &name,
                                                int year,
                                                const features_list &features)
{
  sp_movie cur_movie = std::make_shared<Movie> (name, year);
  _movie_map[cur_movie] = features;
  return cur_movie;
}

double RecommendationSystem::scalar_mult (const features_list
                                          &features_1, const features_list &features_2)
{
  double sum = 0;
  for (size_t i = 0; i < features_1.size (); i++)
  {
    sum += features_1[i] * features_2[i];
  }
  return sum;
}

double RecommendationSystem::calculate_features_similarity (const features_list
&features_1, const features_list &features_2)
{
  double mult_features = scalar_mult (features_1, features_2);
  double norm_f1 = sqrt (scalar_mult (features_1, features_1));
  double norm_f2 = sqrt (scalar_mult (features_2, features_2));
  return mult_features / (norm_f1 * norm_f2);
}

rank_map create_user_norm_rank (rank_map user_rank)
{
  double sum_ranks = 0;
  double count = 0;
  for (auto &pair: user_rank)
  {
    if (pair.second != 0)
    {
      count += 1;
      sum_ranks += pair.second;
    }
  }

  double ranks_average = sum_ranks / count;

  for (auto &pair: user_rank)
  {
    if (pair.second != 0)
    {
      pair.second -= ranks_average;
    }
  }
  return user_rank;
}

sp_movie RecommendationSystem::recommend_by_content (const User &user)
{
  //stage 1: calculate user ranks' average and subtract it from its ranks.
  rank_map norm_rank = create_user_norm_rank (user.get_ranks ());

  features_list user_profile (_movie_map.begin ()->second.size (), 0.0);

  for (const auto &pair: norm_rank)
  {
    if (pair.second != 0)
    {
      const features_list &features = _movie_map.find (pair.first)->second;
      for (size_t i = 0; i < features.size (); i++)
      {
        user_profile[i] += pair.second * features[i];
      }
    }
  }

  sp_movie best_movie = nullptr;
  double highest_score = -1.0;
  for (const auto &pair: _movie_map)
  {
    if (user.get_ranks ().find (pair.first) == user.get_ranks ().end () || user
    .get_ranks ().at (pair.first) == 0)
    {
      double similarity = calculate_features_similarity (user_profile,
                                                         pair.second);
      if (similarity > highest_score)
      {
        highest_score = similarity;
        best_movie = pair.first;
      }
    }
  }
  return best_movie;
}

sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k)
{
  sp_movie best_movie = nullptr;
  bool flag_first_movie = false;
  double highest_predict_score = -1.0;
  for (const auto &pair: _movie_map)
  {
    if (user.get_ranks ().find (pair.first) == user.get_ranks ().end ())
    {
      double predict_score = predict_movie_score (user, pair.first, k);
      if (!flag_first_movie)
      {
        highest_predict_score = predict_score;
        best_movie = pair.first;
        flag_first_movie = true;
      }
      else if (predict_score > highest_predict_score)
      {
        highest_predict_score = predict_score;
        best_movie = pair.first;
      }
    }
  }
  return best_movie;
}

double RecommendationSystem::predict_movie_score (const User &user,
                                                  const sp_movie &movie, int k)
{
  std::vector<std::pair<double, double>> similarity_and_rank;
  for (const auto &it: user.get_ranks ())
  {
    const auto &other_movie = it.first;
    auto movie_it = _movie_map.find (other_movie);
    if (movie_it != _movie_map.end ())
    {
      double similarity = calculate_features_similarity (_movie_map[movie],
                                                         movie_it->second);
      similarity_and_rank.emplace_back (similarity, it.second);
    }
  }
  std::sort (similarity_and_rank.begin (), similarity_and_rank.end (),
             std::greater<> ());

  double numerator = 0.0;
  double denominator = 0.0;
  for (int i = 0; i < k; i++)
  {
    numerator += similarity_and_rank[i].first * similarity_and_rank[i].second;
    denominator += similarity_and_rank[i].first;
  }
  return numerator / denominator;
}

std::shared_ptr<Movie> RecommendationSystem::get_movie (const std::string &
name, int year) const
{
  sp_movie temp_movie = std::make_shared<Movie> (name, year);
  auto it = _movie_map.find (temp_movie);
  if (it != _movie_map.end ())
  {
    return it->first;
  }
  return nullptr;
}

std::ostream &operator<< (std::ostream &os, const RecommendationSystem &rs)
{
  for (const auto &pair: rs._movie_map)
  {
    os << *(pair.first);
  }
  return os;
}