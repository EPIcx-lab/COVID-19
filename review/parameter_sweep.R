# sweep across parameter values

parameter_sweep <- function(scenarios = NULL, samples = NULL,
                            sim_fn = NULL, show_progress = TRUE) {

  safe_sim_fn <- purrr::safely(sim_fn) 

  scenario_sims <- scenarios %>%
    dplyr::group_by(scenario) %>%
    tidyr::nest() %>%
    dplyr::ungroup() %>%
    dplyr::sample_frac(size = 1, replace = FALSE) %>% 
    dplyr::mutate(sims = furrr::future_map( 
      data,
      ~ safe_sim_fn(n.sim = samples,
                    prob_vacc = .$prob_vacc,
                    r0community = .$r0community,
                    prop.asym = .$prop.asym,
                    prop.ascertain = .$prop.ascertain,
                    quarantine = .$quarantine
                    )[[1]],
      .progress = show_progress
    )) %>%
    tidyr::unnest(cols = "data")

  return(scenario_sims)
}



