# run the branching process

#' @param num.initial.cases number of individuals in the 0th generation
#' @param prop.asym proportion of asymptomatic individuals in the unvaccinated subpopulation
#' @param prob_vacc proportion of vaccinated individuals in the branching process 
#' @param delayfn_ parameters of the distribution of behvioral delays
#' @param inf_ parameters of the distribution of generation times
#' @param incfn parameters of the distribution of incubation periods
#' @param incfn_vaccinated
#' @param quarantine parameter that is not used
#' @param disp. dispersion parameters of the distribution of the number of secondary cases
#' @param r0subclin.vaccinated mean number of secondary cases generated by an asymptomatic vaccinated individual
#' @param r0subclin mean number of secondary cases generated by an asymptomatic unvaccianted case
#' @param r0community.vaccinated mean number of secondary cases generated by a symptomatic vaccinated individual
#' @param r0community mean number of secondary cases generated by a symptomatic unvaccinated case
#' @param prop.ascertain probability that an offspring will be traced by its infector if the infector traces its contacts
#' @param test_delay_antigenic delay between test sampling and test result for antigenic tests
#' @param scenario which protocol
#' @param awareness probability that an individual tests a second time if negative but symptomatic
#' @param number_daily_tests maximum number of daily performed tests for individuals who are negative and symptomatic
#' @param comply probability of repeating a test more than twice if negative but symptomatic
#' @param test_compliance_sympt compliance to getting tested if recommended to do so by health authorities, if symptomatic
#' @param test_compliance_asymptcompliance to getting tested if recommended to do so by health authorities, if asymptomatic
#' @param probability_full_iso_sympt compliance to isolation if symptomatic and if recommended by authorities to do so
#' @param probability_full_iso_asympt compliance to isolation if asymptomatic and if recommended by authorities to do so


outbreak_model <- function( num.initial.cases = NULL,
                            run = NULL,
                            prop.ascertain = NULL,
                            prop.asym = NULL,
                            cap_max_days = NULL,
                            prob_vacc = NULL,

                            delay_scale = NULL,
                            delay_shape_sympt = NULL,
                            delay_shape_vaccinated_sympt = NULL,
                            delay_shape_subclin = NULL,
                            delay_shape_vaccinated_subclin = NULL,

                            inc_meanlog = NULL,
                            inc_sdlog = NULL,
                            inc_sdlog_vacc = NULL,
                            inc_meanlog_vacc = NULL,

                            r0isolated = NULL,
                            r0community = NULL,
                            r0community.vaccinated = NULL,
                            disp.com = NULL,
                            disp.com.vaccinated = NULL,
                            r0subclin = NULL,
                            r0subclin.vaccinated = NULL,
                            disp.subclin = NULL,
                            disp.subclin.vaccinated = NULL,

                            inf_shape = NULL,
                            inf_rate = NULL,
                            inf_shift = NULL,
                            inf_shape_vaccinated = NULL,
                            inf_rate_vaccinated = NULL,
                            inf_shift_vaccinated = NULL,
                            quarantine = NULL,
                            test_delay_antigenic = NULL,

                            scenario = NULL,
                            awareness = NULL,
                            number_daily_tests = NULL,
                            comply = NULL,
                            test_compliance_sympt = NULL,
                            test_compliance_asympt = NULL,
                            probability_full_iso_sympt = NULL,
                            probability_full_iso_asympt = NULL
                           ) {

  randomize <- unclass(Sys.time())
  my_seed <- (randomize + run)%%.Machine$integer.max
  set.seed(my_seed)

  delayfn_subclin <- 
    dist_setup(delay_shape_subclin,
                      delay_scale,
                      "adherence")
  delayfn_vaccinated_subclin <- 
    dist_setup(delay_shape_vaccinated_subclin,
                                 delay_scale,
                                 "adherence")

  delayfn_sympt <- 
    dist_setup(delay_shape_sympt,
                    delay_scale,
                    "adherence")
  delayfn_vaccinated_sympt <- 
    dist_setup(delay_shape_vaccinated_sympt,
                               delay_scale,
                               "adherence")
  delayfn <- 
    dist_setup(1,
              delay_scale,
  "adherence")


  incfn <- dist_setup(dist_param1 = inc_meanlog,
            dist_param2 = inc_sdlog,
            dist_type = 'gamma') #lognormal')

  incfn_vaccinated <- dist_setup(dist_param1 = inc_meanlog_vacc,
                       dist_param2 = inc_sdlog_vacc,
                       dist_type = 'gamma') #lognormal')

  # Set initial values for loop indices
  total.cases <- num.initial.cases
  latest.onset <- 0
  extinct <- FALSE

  # Initial setup
  case_data <- outbreak_setup(num.initial.cases = num.initial.cases,
                              prop.asym = prop.asym,
                              prob_vacc = prob_vacc,

                              delayfn_subclin = delayfn_subclin,
                              delayfn_vaccinated_subclin = delayfn_vaccinated_subclin,
                              delayfn_sympt = delayfn_sympt,
                              delayfn_vaccinated_sympt = delayfn_vaccinated_sympt,

                              inf_shape = inf_shape,
                              inf_rate = inf_rate,
                              inf_shift = inf_shift,
                              inf_shape_vaccinated = inf_shape_vaccinated,
                              inf_rate_vaccinated = inf_rate_vaccinated,
                              inf_shift_vaccinated = inf_shift_vaccinated,

                              incfn = incfn,
                              incfn_vaccinated = incfn_vaccinated,

                              quarantine = quarantine,
                              disp.subclin.vaccinated = disp.subclin.vaccinated,
                              disp.subclin = disp.subclin,
                              disp.com.vaccinated = disp.com.vaccinated,
                              disp.com = disp.com,
                              r0subclin.vaccinated = r0subclin.vaccinated,
                              r0subclin = r0subclin,
                              r0community.vaccinated = r0community.vaccinated,
                              r0community = r0community
                              )

  # check whether already extinct
  extinct <- all(case_data$removed)
  if (extinct == TRUE){
    max_week <- floor(cap_max_days / 7)
    missing_weeks <- seq(0, max_week, 1)
    weekly_cases <- data.table(week = missing_weeks, weekly_cases = 0, cumulative = 0, mean_effective_r0 = 0, cases_per_gen = 0, effective_r0_time = 0)
    weekly_cases <- weekly_cases[week <= max_week]
    return(weekly_cases)
  }

  # Preallocate
  effective_r0_vect <- c()
  cases_in_gen_vect <- c(num.initial.cases)
  dist_ind_r0 <- c()

  first_gen <- nrow(case_data)
  count <- 0
  
  a <- 1 # remove the transient part
  t_1 <- c()
  t_2 <- c()
  number_tests <- c()
  days_isolation <- c()
  share_positive_entry <- c()
  share_positive_exit <- c()
  infector_iso_time <- c()
  infector_iso_exit <- c()
  percentage_isolating <- c()
  percentage_traced_iso <- c()
  percentage_sympt <- c()
  percentage_vacc <- c()
  dist_tests <- c()
  individuals_test <- c()
  b <- c()
  
  while (latest.onset < cap_max_days & !extinct) {
    
    out <- outbreak_step(case_data = case_data,

                         prop.asym = prop.asym,
                         prop.ascertain = prop.ascertain,
                         prob_vacc = prob_vacc,

                         incfn = incfn,
                         incfn_vaccinated = incfn_vaccinated,

                         delayfn = delayfn,
                         delayfn_sympt = delayfn_sympt,
                         delayfn_subclin = delayfn_subclin,
                         delayfn_vaccinated_subclin = delayfn_vaccinated_subclin,
                         delayfn_vaccinated_sympt = delayfn_vaccinated_sympt,
                         delay_shape_vaccinated_sympt = delay_shape_vaccinated_sympt,
                         delay_shape_sympt = delay_shape_sympt,
                         delay_shape_vaccinated_subclin = delay_shape_vaccinated_subclin,
                         delay_shape_subclin = delay_shape_subclin,

                         r0isolated = r0isolated,
                         disp.iso = disp.iso,
                         disp.iso.vaccinated = disp.iso.vaccinated,
                         r0community = r0community,
                         r0community.vaccinated = r0community.vaccinated,
                         disp.com = disp.com,
                         disp.com.vaccinated = disp.com.vaccinated,
                         r0subclin = r0subclin,
                         r0subclin.vaccinated = r0subclin.vaccinated,
                         disp.subclin = disp.subclin,
                         disp.subclin.vaccinated = disp.subclin.vaccinated,

                         inf_shape = inf_shape,
                         inf_rate = inf_rate,
                         inf_shift = inf_shift,
                         inf_shape_vaccinated = inf_shape_vaccinated,
                         inf_rate_vaccinated = inf_rate_vaccinated,
                         inf_shift_vaccinated = inf_shift_vaccinated,

                         quarantine = quarantine,
                         test_delay_antigenic = test_delay_antigenic,
                         scenario = scenario,
                         awareness = awareness,
                         number_daily_tests = number_daily_tests,
                         comply = comply,
                         test_compliance_sympt = test_compliance_sympt,
                         test_compliance_asympt = test_compliance_asympt,
                         probability_full_iso_sympt = probability_full_iso_sympt,
                         probability_full_iso_asympt = probability_full_iso_asympt
                        )
   
   if(count > a){
      effective_r0_vect <- c(effective_r0_vect, out[[2]])
      cases_in_gen_vect <- c(cases_in_gen_vect, out[[3]])
      number_tests <- c(number_tests, out[[6]])
      days_isolation <- c(days_isolation, out[[7]])
      infector_iso_time <- c(infector_iso_time, out[[8]])
      infector_iso_exit <- c(infector_iso_exit, out[[9]])
      percentage_isolating <- c(percentage_isolating, out[[10]])
      percentage_traced_iso <- c(percentage_traced_iso, out[[11]])
      percentage_sympt <- c(percentage_sympt, out[[12]])
      percentage_vacc <- c(percentage_vacc, out[[13]])
      t_1 <- c(t_1, out[[14]])
      t_2 <- c(t_2, out[[15]])
      individuals_test <- c(individuals_test, out[[16]])
      b <- c(b, out[[17]])
   }
  
  case_data <- out[[1]]
  total.cases <- nrow(case_data)
  latest.onset <- max(case_data$timeexp)
  extinct <- all(case_data$removed)
  count <- count + 1
  }

  timeexp_vect <- c(case_data$timeexp)

  dist_ind_r0 <- case_data$new_cases[!is.na(case_data$new_cases)]
  dist_ind_r0 <- c(first_gen, dist_ind_r0)

  if (count <= a+1 & extinct == TRUE){
    effective_r0_vect <- 0
    cases_in_gen_vect <- 0
    print("extinct")
  }

  if (count <= a+1 & extinct == FALSE){
    effective_r0_vect <- out[[2]]
    cases_in_gen_vect <- out[[3]]
    print("not extinct but time too short")
  }

  # Prepare output, group into weeks
  weekly_cases <- case_data[, week := floor((timeexp)/ 7)
                            ][, .(weekly_cases = .N), by = week
                              ]

  max_week <- floor(cap_max_days / 7)
  missing_weeks <- (0:max_week)[!(0:max_week %in% weekly_cases$week)]
  if (length(missing_weeks > 0)) {
    weekly_cases <- data.table::rbindlist(list(weekly_cases,
                                               data.table(week = missing_weeks,
                                                          weekly_cases = 0)))
  }
  weekly_cases <- weekly_cases[order(week) # rearranges by increasing order of weeks
                               ][, cumulative := cumsum(weekly_cases)]
  weekly_cases <- weekly_cases[week <= max_week]

  weekly_cases <- weekly_cases[, `:=`(mean_effective_r0 = mean(effective_r0_vect,
                                                               na.rm = TRUE),
                                      cases_per_gen = list(cases_in_gen_vect),
                                      timeexp_vect = list(timeexp_vect),
                                      effective_r0_time = list(effective_r0_vect),
                                      t_1 = list(t_1),
                                      t_2 = list(t_2),
                                      number_tests = list(number_tests),
                                      days_isolation = list(days_isolation),
                                      infector_iso_exit = list(infector_iso_exit),
                                      infector_iso_time = list(infector_iso_time),
                                      percentage_isolating = list(percentage_isolating),
                                      percentage_traced_iso = list(percentage_traced_iso),
                                      percentage_sympt = list(percentage_sympt),
                                      percentage_vacc = list(percentage_vacc),
                                      individuals_test = list(individuals_test),
                                      b = list(b)
                                      )]
  
  only_first <- which(weekly_cases$week == 0)
  weekly_cases <- weekly_cases[only_first]
  
  return(weekly_cases)
}
