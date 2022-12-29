# generate results with specific parameter choices
library(tidyverse)
library(magrittr)
library(data.table)

#' @param n.sim number of simulations to be run
#' @param r0community intrinsic transmissibility of the virus
#' @param scenario desired testing and isolation protocol, among those implemented in France, Belgium and Italy in early 2022
#' @param ad country's prospective adherence to testing and isolation among France, Belgium and Italy
#' @param cost cost conditions, whether "normal" or "less_20_euros", to tune adherence to testing and isolation based on test cost conditions
#' @param num.initial.cases number of branching trees growing in parallel
#' @param cap_max_days maximum number of days to run the simulations for
#' @param prop.asym probability of being asymptomatic, when unvaccinated
#' @param prob_vacc vaccine uptake in the population
#' @param r0subclin relative infectiousness of asymptomatic individiuals compared to symptomatic ones
#' @param vaccine_transmission_reduction relative transmissibility of vaccinated individuals compared to unvaccinated ones
#' @param disp.com dispersion parameter of the distribution of number of secondary cases for symptomatic & unvaccinated individuals
#' @param disp.subclin dispersion parameter of the distribution of number of secondary cases for asymptomatic & unvaccinated individuals
#' @param disp.com.vaccinated dispersion parameter of the distribution of number of secondary cases for symptomatic & vaccinated individuals
#' @param disp.subclin.vaccinated dispersion parameter of the distribution of number of secondary cases for asymptomatic & vaccinated individuals
#' @param inc_meanlog parameter 1 of the incubation period distribution (default distribution is gamma) for unvaccinated individuals
#' @param inc_sdlog parameter 2 of the incubation period distribution (default distribution is gamma) for unvaccinated individuals
#' @param inc_meanlog_vacc parameter 1 of the incubation period distribution (default distribution is gamma) for vaccinated individuals
#' @param inc_sdlog_vacc parameter 2 of the incubation period distribution (default distribution is gamma) for vaccinated individuals
#' @param inf_shape parameter 1 of the generation time distribution (default distribution is gamma), if unvaccinated
#' @param inf_shift shift of the generation time distribution (default distribution is gamma), if any, if unvaccinated
#' @param inf_rate parameter 2 of the generation time distribution (default distribution is gamma), if unvaccinated
#' @param inf_shape_vaccinated parameter 1 of the generation time distribution (default distribution is gamma), if vaccinated
#' @param inf_shift_vaccinated shift of the generation time distribution (default distribution is gamma), if any, if vaccinated
#' @param inf_rate_vaccinated parameter 2 of the generation time distribution (default distribution is gamma), if vaccinated
#' @param test_delay_antigenic delay in days between test sampling and test result
#' @param number_daily_tests maximum number of daily tests performed if symptomatic but presistently negative
#' @param prop.ascertain share of offspring of a tracing infector that will be traced
#' @param r0isolated isolation effectiveness - probability of infecting an offspring during the isolation period

n.sim = 10
r0community = 0.65
scenario = 1
ad = "France"
cost = "normal"

if (cost == "normal"){
  if (ad == "France"){
  test_compliance_sympt = 92.7/100
  test_compliance_asympt = 78.2/100
  comply = 50.9/100 # probabilty of repeating test a second time
  awareness = 5.8/100 # probabilty of repeating test more than three times
  probability_full_iso_sympt = 98.3/100
  probability_full_iso_asympt = 97.9/100
  delay_shape_sympt = 78.0/100
  delay_shape_subclin = 72.3/100
  delay_shape_vaccinated_sympt = 96.3/100
  delay_shape_vaccinated_subclin = 88.5/100
  delay_scale = 1.1
}
  if (ad == "Belgium"){
    test_compliance_sympt = 91.2/100
    test_compliance_asympt = 82.3/100
    comply = 56.7/100 # probabilty of repeating test a second time
    awareness = 12.2/100 # probabilty of repeating test more than three times
    probability_full_iso_sympt = 99.2/100
    probability_full_iso_asympt = 98.4/100
    delay_shape_sympt = 76.9/100
    delay_shape_subclin = 76.5/100
    delay_shape_vaccinated_sympt = 95.0/100
    delay_shape_vaccinated_subclin = 93.6/100
    delay_scale = 1.1
  }
  if (ad == "Italy"){
    test_compliance_sympt = 97.3/100
    test_compliance_asympt = 89.6/100
    comply = 68.2/100 # probabilty of repeating test a second time
    awareness = 15.7/100 # probabilty of repeating test more than three times
    probability_full_iso_sympt = 97.9/100
    probability_full_iso_asympt = 99.1/100
    delay_shape_sympt = 78.3/100
    delay_shape_subclin = 79.4/100
    delay_shape_vaccinated_sympt = 96.7/100
    delay_shape_vaccinated_subclin = 97.2/100
    delay_scale = 1.1
  }
}
if (cost == "less_20_euros"){
  if (ad == "France"){
    test_compliance_sympt = 45.48/100
    test_compliance_asympt = 41.97/100
    comply = 50.9/92.7*45.48/100 # probabilty of repeating test a second time
    awareness = 5.8/92.7*45.48/100 # probabilty of repeating test more than three times
    probability_full_iso_sympt = 98.3/100
    probability_full_iso_asympt = 97.9/100
    delay_shape_sympt = 78.0/100
    delay_shape_subclin = 72.3/100
    delay_shape_vaccinated_sympt = 96.3/100
    delay_shape_vaccinated_subclin = 88.5/100
    delay_scale = 1.1
  }
  if (ad == "Belgium"){
    test_compliance_sympt = 51.71/100
    test_compliance_asympt = 46.55/100
    comply = 56.7/91.2*51.71/100 #56.7/100 # probabilty of repeating test a second time
    awareness = 12.2/91.2*51.71/100 #12.2/100 # probabilty of repeating test more than three times
    probability_full_iso_sympt = 99.2/100
    probability_full_iso_asympt = 98.4/100
    delay_shape_sympt = 76.9/100
    delay_shape_subclin = 76.5/100
    delay_shape_vaccinated_sympt = 95.0/100
    delay_shape_vaccinated_subclin = 93.6/100
    delay_scale = 1.1
  }
  if (ad == "Italy"){
    test_compliance_sympt = 74.55/100
    test_compliance_asympt = 77.22/100
    comply = 68.2/97.3*74.55/100 # probabilty of repeating test a second time
    awareness = 15.7/97.3*74.55/100 # probabilty of repeating test more than three times
    probability_full_iso_sympt = 97.9/100
    probability_full_iso_asympt = 99.1/100
    delay_shape_sympt = 78.3/100
    delay_shape_subclin = 79.4/100
    delay_shape_vaccinated_sympt = 96.7/100
    delay_shape_vaccinated_subclin = 97.2/100
    delay_scale = 1.1
  }
}

num.initial.cases = 20
cap_max_days = 80
prop.asym = 0.42
prob_vacc = 0.76

r0subclin = 0.55
vaccine_transmission_reduction = 0.05

disp.com = 0.23
disp.subclin = 0.23
disp.com.vaccinated = 0.23
disp.subclin.vaccinated = 0.23

inc_meanlog = 8.50 #1.245956
inc_sdlog = 1/0.41 #0.5872772
inc_meanlog_vacc = 8.50 #1.245956
inc_sdlog_vacc = 1/0.41 #0.5872772

inf_shape = 2.39 #0.9517723
inf_shift =  0
inf_rate = 1/2.95 #0.5313171
inf_shape_vaccinated = 2.39 #0.9517723
inf_shift_vaccinated = 0
inf_rate_vaccinated = 1/2.95 #0.5313171

test_delay_antigenic = 0.01
number_daily_tests = 4

prop.ascertain = 0.05
r0isolated = 0.4

sim_with_params <- purrr::partial(scenario_sim,
                                  num.initial.cases = num.initial.cases,
                                  cap_max_days = cap_max_days,
                                  prop.asym = prop.asym,
                                  prob_vacc = prob_vacc,
                                  quarantine = quarantine,

                                  r0community = r0community,
                                  r0subclin = r0subclin,
                                  vaccine_transmission_reduction = vaccine_transmission_reduction,

                                  disp.com = disp.com,
                                  disp.subclin = disp.subclin,
                                  disp.com.vaccinated = disp.com.vaccinated,
                                  disp.subclin.vaccinated = disp.subclin.vaccinated,

                                  inc_meanlog = inc_meanlog, 
                                  inc_sdlog = inc_sdlog, 
                                  inc_meanlog_vacc = inc_meanlog_vacc,
                                  inc_sdlog_vacc = inc_sdlog_vacc,

                                  inf_shape = inf_shape,
                                  inf_shift =  inf_shift,
                                  inf_rate = inf_rate,
                                  inf_shape_vaccinated = inf_shape_vaccinated,
                                  inf_shift_vaccinated = inf_shift_vaccinated,
                                  inf_rate_vaccinated = inf_rate_vaccinated, 

                                  scenario = scenario,

                                  test_delay_antigenic = test_delay_antigenic,
                                  number_daily_tests = number_daily_tests,
                                  test_compliance_sympt = test_compliance_sympt,
                                  test_compliance_asympt = test_compliance_asympt,
                                  comply = comply, 
                                  awareness = awareness,

                                  prop.ascertain = prop.ascertain,

                                  probability_full_iso_sympt = probability_full_iso_sympt,
                                  probability_full_iso_asympt = probability_full_iso_asympt,
                                  delay_shape_sympt = delay_shape_sympt,
                                  delay_shape_subclin = delay_shape_subclin,
                                  delay_shape_vaccinated_sympt = delay_shape_vaccinated_sympt,
                                  delay_shape_vaccinated_subclin = delay_shape_vaccinated_subclin,
                                  delay_scale = delay_scale,
                                  r0isolated = r0isolated
)

future::plan("multisession")

## Run parameter sweep
sweep_results_baseline <- sim_with_params(n.sim = n.sim)

