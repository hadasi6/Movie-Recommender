#include "User.h"

#include <utility>
#include "RecommendationSystem.h"

User::User (std::string  user_name, rank_map& r_map,
            std::shared_ptr<RecommendationSystem>& rs): _user_name
            (std::move(user_name)), _rank_map(r_map), _system(rs) {}

std::string User::get_name () const
{
  return _user_name;
}

void User::add_movie_to_user (const std::string &name, int year,
                              const features_list &features, double rate)
{
  sp_movie cur_new_movie = _system->add_movie_to_rs (name, year, features);
  _rank_map[cur_new_movie] = rate;
}

rank_map User::get_ranks () const
{
  return _rank_map;
}

sp_movie User::get_rs_recommendation_by_content () const
{
  return _system->recommend_by_content (*this);
}

sp_movie User::get_rs_recommendation_by_cf (int k) const
{
  return _system->recommend_by_cf (*this, k);
}

double User::get_rs_prediction_score_for_movie (const std::string &name, int
                                                  year, int k) const
{
  return _system->predict_movie_score
                  (*this, std::make_shared<Movie> (name,year),k);
}

std::ostream& operator<<(std::ostream &os, const User &user)
{
  os << "name: "<< user.get_name() <<"\n";
  os << user._system<< std::endl;
  return os;
}
