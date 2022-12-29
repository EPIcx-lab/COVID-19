# define useful functions
library(tidyverse)
library(lubridate)
library(patchwork)

# useful distributions
dist_setup <- function(dist_param1 = NULL, dist_param2 = NULL, dist_type = NULL) {

  if(dist_type == "weibull"){
    out <- purrr::partial(rweibull,
                          shape = dist_param1,
                          scale = dist_param2)
  }
  if(dist_type == "gamma"){
    out <- purrr::partial(rgamma,
                          shape = dist_param1,
                          rate = dist_param2)
  }
  if(dist_type == "lognormal"){
    out <- purrr::partial(rlnorm,
                          meanlog = dist_param1,
                          sdlog = dist_param2)
  }
  if(dist_type == "adherence"){
    out <- purrr::partial(adherence,
                          p = dist_param1,
                          mu = dist_param2)
  }
  if(dist_type == "constant"){
    out <- purrr::partial(runif,
                          min = 1,
                          max  =1)
  }
  return(out)
}

# delay distribution with imperfect adherence p
adherence <- function(n, p, mu){
  iso_delay <- rexp(n, 1/mu)
  ifelse(as.logical(rbinom(n, 1, p)), iso_delay, Inf)
}

# anticipated exit from isolation
isolation_duration <- function(n, X, prob){
  anticipated <- rexp(n, 4/(3*X))
  ifelse(as.logical(rbinom(n, 1, prob)), 
         rep(X, n),
         ifelse((X - anticipated) > 1,
                    X - anticipated,
                    1))
}

# generation time distribution
inf_fn <- function(n=NULL, inf_shape = NULL, inf_rate = NULL, inf_shift = NULL) {
  if (length(n)==0){out <- c()}
  if(length(n)!=0){
      out <- rep(NA, length(n))
      for(i in 1: length(n)){
        a <- n[i]
        if(a != 0){
          out[i] <- list(- inf_shift + 
                           # rlnorm(a,
                           #                    meanlog = inf_shape,
                           #                    sdlog = inf_rate))
                           rgamma(a,
                                         shape = inf_shape,
                                         rate = inf_rate))
        }
      }
  }
  return(out)
  }

# choose the country's immunity profile to run the simulations with
vacc_coverage <- "France"

if (vacc_coverage == "France"){
  # draw individual VE against infection
  prob_immune <- function(n=NULL){ 
    # standard deviations of the VE in each waning category
    standard_deviation_inf <- function(number_dose, range_low, range_up){
      aux <- matrix(0, 2, 9)
      aux[1,] <- c(64.43681, NA, 23.25086, 15.29508, NA, 9.46434, 8.064979, 4.990475, 2.242365) # second dose
      aux[2,] <- c(62.50715, 40.1821, 29.7977, 5.40298, 0.7312138, NA, 0.001812498, NA, 0) # third dose
      
      if (range_low == 1/4){index_time_low <- 1}
      if (range_low == 3){index_time_low <- 2}
      if (range_low == 4){index_time_low <- 3}
      if (range_low == 6){index_time_low <- 4}
      if (range_low == 7){index_time_low <- 5}
      if (range_low == 9){index_time_low <- 6}
      if (range_low == 10){index_time_low <- 7}
      if (range_low == 13){index_time_low <- 8}
      if (range_low == 18){index_time_low <- 9}
      
      if (range_up == 1/4){index_time_up <- 1}
      if (range_up == 3){index_time_up <- 2}
      if (range_up == 4){index_time_up <- 3}
      if (range_up == 6){index_time_up <- 4}
      if (range_up == 7){index_time_up <- 5}
      if (range_up == 9){index_time_up <- 6}
      if (range_up == 10){index_time_up <- 7}
      if (range_up == 13){index_time_up <- 8}
      if (range_up == 18){index_time_up <- 9}
      
      out <- (aux[number_dose-1, index_time_low] - aux[number_dose-1, index_time_up])/6
      return(out)
    }
    
    N <- 100
    gaussian <- function(x, mu, sd){
      out <- exp(- (x-mu)^2/(2*sd^2))/(sd*sqrt(2*pi))
      return(out)
    }
    
    gaussian_2_doses_6 <- function(x){
      out <- gaussian(x, mu=6.277996/100, sd=standard_deviation_inf(2, 10, 13)/100)*0.1006946 
      out
    }
    gaussian_3_doses_6 <- function(x){
      mu <- (0.0000001/100 + 3.366893/100 + 0.093697/100)/3 
      out <- exp(-x/mu)*1/mu*(0.4037377 + 0.1710956 + 0.01491028)
      out
    }
    gaussian_2_doses_now <- function(x){
      out <- gaussian(x, mu=13.38296/100, sd=standard_deviation_inf(2, 4, 10)/100)*0.008993048
      out
    }
    gaussian_3_doses_now <- function(x){
      out <- gaussian(x, mu=11.58163/100, sd=standard_deviation_inf(3, 4, 7)/100)*0.00953047
      out
    }
    gaussian_3_doses_senior <- function(x){
      out <- gaussian(x, mu=47.47437/100, sd=standard_deviation_inf(3, 1/4, 4)/100)*0.1846023
      out
    }
    gaussian_4_doses <- function(x){
      out <- gaussian(x, mu=11.58163/100, sd=standard_deviation_inf(3, 4, 7)/100)*0.1064358
      out
    }
    sum_gaussians <- function(x){
      out <- gaussian_2_doses_6(x) + gaussian_2_doses_now(x) + gaussian_3_doses_now(x) + gaussian_3_doses_6(x) + gaussian_3_doses_senior(x) + gaussian_4_doses(x)
      return(out)
    }
    
    out <- data.table::data.table(x = seq(0, 1, 0.0005))
    out[, `:=`(
      sum_gaussians = sum_gaussians(x)
    ), ]
    out %<>% tidyr::gather("incub", "value", -x)
    
    dist <- c()
    for (i in 1:(N+1)){
      x <- (i-1)/N
      dist[[i]] <- sum_gaussians(x)
    }
    max_sum_gaussians <- max(as.numeric(dist))
    mean_sum_gaussians <- 0
    for (i in 1:(N+1)){
      mean_sum_gaussians <- mean_sum_gaussians + (i-1)/N*dist[[i]]*1/N
    }
    mean_sum_gaussians*100
    
    out <- c()
    for (i in 1:n){
      end <- 0
      while(end == 0){
        X <- runif(1, min=0, max=1)
        Y <- runif(1, min=0, max=max_sum_gaussians)
        if (sum_gaussians(X) >= Y){ 
          out[[i]] <- X
          end <- 1
        }
        end
      }
    }
    
    return(list(unlist(out), mean(unlist(out))))
  }
  
  prop.asym.vaccinated <- function(n=NULL, prop.asym=NULL){ 
    # standard deviation of VE in each waning category
    standard_deviation_sympt <- function(number_dose, range_low, range_up){
      aux <- matrix(0, 2, 9)
      aux[1,] <- c(71.59646, NA, 25.83551, 16.99614, NA, 10.51693, 8.961934, 5.545496, 2.491752) # second dose
      aux[2,] <- c(69.45239, 44.64629, 33.10769, 6.00137, 0.8121971, NA, 0.002013335, NA, 0) # third dose
      
      if (range_low == 1/4){index_time_low <- 1}
      if (range_low == 3){index_time_low <- 2}
      if (range_low == 4){index_time_low <- 3}
      if (range_low == 6){index_time_low <- 4}
      if (range_low == 7){index_time_low <- 5}
      if (range_low == 9){index_time_low <- 6}
      if (range_low == 10){index_time_low <- 7}
      if (range_low == 13){index_time_low <- 8}
      if (range_low == 18){index_time_low <- 9}
      
      if (range_up == 1/4){index_time_up <- 1}
      if (range_up == 3){index_time_up <- 2}
      if (range_up == 4){index_time_up <- 3}
      if (range_up == 6){index_time_up <- 4}
      if (range_up == 7){index_time_up <- 5}
      if (range_up == 9){index_time_up <- 6}
      if (range_up == 10){index_time_up <- 7}
      if (range_up == 13){index_time_up <- 8}
      if (range_up == 18){index_time_up <- 9}
      
      out <- (aux[number_dose-1, index_time_low] - aux[number_dose-1, index_time_up])/6
      return(out)
    }
    
    N <- 100
    gaussian <- function(x, mu, sd){
      out <- exp(- (x-mu)^2/(2*sd^2))/(sd*sqrt(2*pi))
      return(out)
    }
    
    gaussian_2_doses_6 <- function(x){
      out <- gaussian(x, mu=6.976176/100, sd=standard_deviation_sympt(2, 10, 13)/100)*0.1006946
      out
    }
    gaussian_3_doses_6 <- function(x){
      mu <- (0.0000001/100 + 3.7413450/100 + 0.1040765/100)/3
      out <- exp(-x/mu)*1/mu*(0.4037377 + 0.1710956 + 0.01491028)
      out
    }
    gaussian_2_doses_now <- function(x){
      out <- gaussian(x, mu=14.87129/100, sd=standard_deviation_sympt(2, 4, 10)/100)*0.008993048
      out
    }
    gaussian_3_doses_now <- function(x){
      out <- gaussian(x, mu=12.8673/100, sd=standard_deviation_sympt(3, 4, 7)/100)*0.009530470
      out
    }
    gaussian_3_doses_senior <- function(x){
      out <- gaussian(x, mu=52.7489900/100, sd=standard_deviation_sympt(3, 1/4, 4)/100)*0.184602324 
      out
    }
    gaussian_4_doses <- function(x){
      out <- gaussian(x, mu=12.8673/100, sd=standard_deviation_sympt(3, 4, 7)/100)*0.106435840
      out
    }
    sum_gaussians <- function(x){
      out <- gaussian_2_doses_6(x) + gaussian_2_doses_now(x) + gaussian_3_doses_now(x) + gaussian_3_doses_6(x) + gaussian_3_doses_senior(x) + gaussian_4_doses(x)
      return(out)
    }
    
    out <- data.table::data.table(x = seq(0, 1, 0.0005))
    out[, `:=`(
      sum_gaussians = sum_gaussians(x)
    ), ]
    out %<>% tidyr::gather("incub", "value", -x)
    
    dist <- c()
    for (i in 1:(N+1)){
      x <- (i-1)/N
      dist[[i]] <- sum_gaussians(x)
    }
    max_sum_gaussians <- max(as.numeric(dist))
    mean_sum_gaussians <- 0
    for (i in 1:(N+1)){
      mean_sum_gaussians <- mean_sum_gaussians + (i-1)/N*dist[[i]]*1/N
    }
    mean_sum_gaussians*100
    
    out <- c()
    for (i in 1:n){
      end <- 0
      while(end == 0){
        X <- runif(1, min=0, max=1)
        Y <- runif(1, min=0, max=max_sum_gaussians)
        if (sum_gaussians(X) >= Y){
          out[[i]] <- 0.1*X/(1-0.9*X) + prop.asym*(1 - 0.1*X/(1-0.9*X))
          end <- 1
        }
        end
      }
    }
    
    return(list(unlist(out), mean(unlist(out)))) 
  }
}

if (vacc_coverage == "Belgium"){
  prob_immune <- function(n=NULL){
    # standard deviation of the VE in the waning category
    standard_deviation_inf <- function(number_dose, range_low, range_up){
      aux <- matrix(0, 2, 9)
      aux[1,] <- c(64.43681, NA, 23.25086, 15.29508, NA, 9.46434, 8.064979, 4.990475, 2.242365) # second dose
      aux[2,] <- c(62.50715, 40.1821, 29.7977, 5.40298, 0.7312138, NA, 0.001812498, NA, 0) # third dose
      
      if (range_low == 1/4){index_time_low <- 1}
      if (range_low == 3){index_time_low <- 2}
      if (range_low == 4){index_time_low <- 3}
      if (range_low == 6){index_time_low <- 4}
      if (range_low == 7){index_time_low <- 5}
      if (range_low == 9){index_time_low <- 6}
      if (range_low == 10){index_time_low <- 7}
      if (range_low == 13){index_time_low <- 8}
      if (range_low == 18){index_time_low <- 9}
      
      if (range_up == 1/4){index_time_up <- 1}
      if (range_up == 3){index_time_up <- 2}
      if (range_up == 4){index_time_up <- 3}
      if (range_up == 6){index_time_up <- 4}
      if (range_up == 7){index_time_up <- 5}
      if (range_up == 9){index_time_up <- 6}
      if (range_up == 10){index_time_up <- 7}
      if (range_up == 13){index_time_up <- 8}
      if (range_up == 18){index_time_up <- 9}
      
      out <- (aux[number_dose-1, index_time_low] - aux[number_dose-1, index_time_up])/6
      return(out)
    }
    
    N <- 100
    gaussian <- function(x, mu, sd){
      out <- exp(- (x-mu)^2/(2*sd^2))/(sd*sqrt(2*pi))
      return(out)
    }
    
    gaussian_2_doses_6 <- function(x){
      out <- gaussian(x, mu=6.277966/100, sd=standard_deviation_inf(2, 10, 13)/100)*0.03771178
      out
    }
    gaussian_3_doses_6 <- function(x){
      mu <- (0.0000001/100 + 3.366893/100 + 0.093697/100)/3 
      out <- exp(-x/mu)*1/mu*(0.5083194 + 0.1661928 + 0.009613367)
      out
    }
    gaussian_2_doses_now <- function(x){
      out <- gaussian(x, mu=13.38296/100, sd=standard_deviation_inf(2, 4, 10)/100)*0.007219591
      out
    }
    gaussian_3_doses_now <- function(x){
      out <- gaussian(x, mu=11.58163/100, sd=standard_deviation_inf(3, 4, 7)/100)*0.003502234
      out
    }
    gaussian_3_doses_senior <- function(x){
      out <- gaussian(x, mu= 47.47437/100, sd=standard_deviation_inf(3, 1/4, 4)/100)*0.1995414 
      out
    }
    gaussian_4_doses <- function(x){
      out <- gaussian(x, mu=11.58163/100, sd=standard_deviation_inf(3, 4, 7)/100)*0.06789946 
      out
    }
    sum_gaussians <- function(x){
      out <- gaussian_2_doses_6(x) + gaussian_2_doses_now(x) + gaussian_3_doses_now(x) + gaussian_3_doses_6(x) + gaussian_3_doses_senior(x) + gaussian_4_doses(x)
      return(out)
    }
    
    out <- data.table::data.table(x = seq(0, 1, 0.0005))
    out[, `:=`(
      sum_gaussians = sum_gaussians(x)
    ), ]
    out %<>% tidyr::gather("incub", "value", -x)
    
    dist <- c()
    for (i in 1:(N+1)){
      x <- (i-1)/N
      dist[[i]] <- sum_gaussians(x)
    }
    max_sum_gaussians <- max(as.numeric(dist))
    mean_sum_gaussians <- 0
    for (i in 1:(N+1)){
      mean_sum_gaussians <- mean_sum_gaussians + (i-1)/N*dist[[i]]*1/N
    }
    mean_sum_gaussians*100
    
    out <- c()
    for (i in 1:n){
      end <- 0
      while(end == 0){
        X <- runif(1, min=0, max=1)
        Y <- runif(1, min=0, max=max_sum_gaussians)
        if (sum_gaussians(X) >= Y){ 
          out[[i]] <- X
          end <- 1
        }
        end
      }
    }
    
    return(list(unlist(out), mean(unlist(out))))
  }
  
  prop.asym.vaccinated <- function(n=NULL, prop.asym=NULL){
    ## standard deviation of VE in waning category
    standard_deviation_sympt <- function(number_dose, range_low, range_up){
      aux <- matrix(0, 2, 9)
      aux[1,] <- c(71.59646, NA, 25.83551, 16.99614, NA, 10.51693, 8.961934, 5.545496, 2.491752) 
      aux[2,] <- c(69.45239, 44.64629, 33.10769, 6.00137, 0.8121971, NA, 0.002013335, NA, 0)
      
      if (range_low == 1/4){index_time_low <- 1}
      if (range_low == 3){index_time_low <- 2}
      if (range_low == 4){index_time_low <- 3}
      if (range_low == 6){index_time_low <- 4}
      if (range_low == 7){index_time_low <- 5}
      if (range_low == 9){index_time_low <- 6}
      if (range_low == 10){index_time_low <- 7}
      if (range_low == 13){index_time_low <- 8}
      if (range_low == 18){index_time_low <- 9}
      
      if (range_up == 1/4){index_time_up <- 1}
      if (range_up == 3){index_time_up <- 2}
      if (range_up == 4){index_time_up <- 3}
      if (range_up == 6){index_time_up <- 4}
      if (range_up == 7){index_time_up <- 5}
      if (range_up == 9){index_time_up <- 6}
      if (range_up == 10){index_time_up <- 7}
      if (range_up == 13){index_time_up <- 8}
      if (range_up == 18){index_time_up <- 9}
      
      out <- (aux[number_dose-1, index_time_low] - aux[number_dose-1, index_time_up])/6
      return(out)
    }
    
    N <- 100
    gaussian <- function(x, mu, sd){
      out <- exp(- (x-mu)^2/(2*sd^2))/(sd*sqrt(2*pi))
      return(out)
    }
    
    gaussian_2_doses_6 <- function(x){
      out <- gaussian(x, mu=6.976176/100, sd=standard_deviation_sympt(2, 10, 13)/100)*0.03771178
      out
    }
    gaussian_3_doses_6 <- function(x){
      mu <- (0.0000001/100 + 3.741345/100 + 0.1040765/100)/3 
      out <- exp(-x/mu)*1/mu*(0.5083194 + 0.1661928 + 0.009613367)
      out
    }
    gaussian_2_doses_now <- function(x){
      out <- gaussian(x, mu=14.87129/100, sd=standard_deviation_sympt(2, 4, 10)/100)*0.007219591
      out
    }
    gaussian_3_doses_now <- function(x){
      out <- gaussian(x, mu=12.8673/100, sd=standard_deviation_sympt(3, 4, 7)/100)*0.003502234
      out
    }
    gaussian_3_doses_senior <- function(x){
      out <- gaussian(x, mu=52.74899/100, sd=standard_deviation_sympt(3, 1/4, 4)/100)*0.1995414
      out
    }
    gaussian_4_doses <- function(x){
      out <- gaussian(x, mu=12.8673/100, sd=standard_deviation_sympt(3, 4, 7)/100)*0.06789946
      out
    }
    sum_gaussians <- function(x){
      out <- gaussian_2_doses_6(x) + gaussian_2_doses_now(x) + gaussian_3_doses_now(x) + gaussian_3_doses_6(x) + gaussian_3_doses_senior(x) + gaussian_4_doses(x)
      return(out)
    }
    
    out <- data.table::data.table(x = seq(0, 1, 0.0005))
    out[, `:=`(
      sum_gaussians = sum_gaussians(x)
    ), ]
    out %<>% tidyr::gather("incub", "value", -x)
    
    dist <- c()
    for (i in 1:(N+1)){
      x <- (i-1)/N
      dist[[i]] <- sum_gaussians(x)
    }
    max_sum_gaussians <- max(as.numeric(dist))
    mean_sum_gaussians <- 0
    for (i in 1:(N+1)){
      mean_sum_gaussians <- mean_sum_gaussians + (i-1)/N*dist[[i]]*1/N
    }
    mean_sum_gaussians*100
    
    out <- c()
    for (i in 1:n){
      end <- 0
      while(end == 0){
        X <- runif(1, min=0, max=1)
        Y <- runif(1, min=0, max=max_sum_gaussians)
        if (sum_gaussians(X) >= Y){
          out[[i]] <- 0.1*X/(1-0.9*X) + prop.asym*(1 - 0.1*X/(1-0.9*X))
          end <- 1
        }
        end
      }
    }
   
    return(list(unlist(out), mean(unlist(out))))
  }
}

if (vacc_coverage == "Italy"){
  prob_immune <- function(n=NULL){ 
    standard_deviation_inf <- function(number_dose, range_low, range_up){
      aux <- matrix(0, 2, 9)
      aux[1,] <- c(64.43681, NA, 23.25086, 15.29508, NA, 9.46434, 8.064979, 4.990475, 2.242365)
      aux[2,] <- c(62.50715, 40.1821, 29.7977, 5.40298, 0.7312138, NA, 0.001812498, NA, 0)
      
      if (range_low == 1/4){index_time_low <- 1}
      if (range_low == 3){index_time_low <- 2}
      if (range_low == 4){index_time_low <- 3}
      if (range_low == 6){index_time_low <- 4}
      if (range_low == 7){index_time_low <- 5}
      if (range_low == 9){index_time_low <- 6}
      if (range_low == 10){index_time_low <- 7}
      if (range_low == 13){index_time_low <- 8}
      if (range_low == 18){index_time_low <- 9}
      
      if (range_up == 1/4){index_time_up <- 1}
      if (range_up == 3){index_time_up <- 2}
      if (range_up == 4){index_time_up <- 3}
      if (range_up == 6){index_time_up <- 4}
      if (range_up == 7){index_time_up <- 5}
      if (range_up == 9){index_time_up <- 6}
      if (range_up == 10){index_time_up <- 7}
      if (range_up == 13){index_time_up <- 8}
      if (range_up == 18){index_time_up <- 9}
      
      out <- (aux[number_dose-1, index_time_low] - aux[number_dose-1, index_time_up])/6
      return(out)
    }
    
    N <- 100
    gaussian <- function(x, mu, sd){
      out <- exp(- (x-mu)^2/(2*sd^2))/(sd*sqrt(2*pi))
      return(out)
    }
    
    gaussian_2_doses_6 <- function(x){
      out <- gaussian(x, mu=6.277966/100, sd=standard_deviation_inf(2, 10, 13)/100)*0.07579852
      out
    }
    gaussian_3_doses_6 <- function(x){
      mu <- (0.0000001/100 + 3.366893/100 + 0.093697/100)/3 
      out <- exp(-x/mu)*1/mu*(0.48689658 + 0.07026471 + 0.03184674)
      out
    }
    gaussian_2_doses_now <- function(x){
      out <- gaussian(x, mu=13.38296/100, sd=standard_deviation_inf(2, 4, 10)/100)*0.01198989
      out
    }
    gaussian_3_doses_now <- function(x){
      out <- gaussian(x, mu=11.58163/100, sd=standard_deviation_inf(3, 4, 7)/100)*0.01279443
      out
    }
    gaussian_3_doses_senior <- function(x){
      out <- gaussian(x, mu=47.47437/100, sd=standard_deviation_inf(3, 1/4, 4)/100)*0.26335747
      out
    }
    gaussian_4_doses <- function(x){
      out <- gaussian(x, mu=11.58163/100, sd=standard_deviation_inf(3, 4, 7)/100)*0.04705167
      out
    }
    sum_gaussians <- function(x){
      out <- gaussian_2_doses_6(x) + gaussian_2_doses_now(x) + gaussian_3_doses_now(x) + gaussian_3_doses_6(x) + gaussian_3_doses_senior(x) + gaussian_4_doses(x)
      return(out)
    }
    
    out <- data.table::data.table(x = seq(0, 1, 0.0005))
    out[, `:=`(
      sum_gaussians = sum_gaussians(x)
    ), ]
    out %<>% tidyr::gather("incub", "value", -x)
    
    dist <- c()
    for (i in 1:(N+1)){
      x <- (i-1)/N
      dist[[i]] <- sum_gaussians(x)
    }
    max_sum_gaussians <- max(as.numeric(dist))
    mean_sum_gaussians <- 0
    for (i in 1:(N+1)){
      mean_sum_gaussians <- mean_sum_gaussians + (i-1)/N*dist[[i]]*1/N
    }
    mean_sum_gaussians*100
    
    out <- c()
    for (i in 1:n){
      end <- 0
      while(end == 0){
        X <- runif(1, min=0, max=1)
        Y <- runif(1, min=0, max=max_sum_gaussians)
        if (sum_gaussians(X) >= Y){
          out[[i]] <- X
          end <- 1
        }
        end
      }
    }
    
    return(list(unlist(out), mean(unlist(out))))
  }
  
  prop.asym.vaccinated <- function(n=NULL, prop.asym=NULL){ 
    standard_deviation_sympt <- function(number_dose, range_low, range_up){
      aux <- matrix(0, 2, 9)
      aux[1,] <- c(71.59646, NA, 25.83551, 16.99614, NA, 10.51693, 8.961934, 5.545496, 2.491752)
      aux[2,] <- c(69.45239, 44.64629, 33.10769, 6.00137, 0.8121971, NA, 0.002013335, NA, 0)
      
      if (range_low == 1/4){index_time_low <- 1}
      if (range_low == 3){index_time_low <- 2}
      if (range_low == 4){index_time_low <- 3}
      if (range_low == 6){index_time_low <- 4}
      if (range_low == 7){index_time_low <- 5}
      if (range_low == 9){index_time_low <- 6}
      if (range_low == 10){index_time_low <- 7}
      if (range_low == 13){index_time_low <- 8}
      if (range_low == 18){index_time_low <- 9}
      
      if (range_up == 1/4){index_time_up <- 1}
      if (range_up == 3){index_time_up <- 2}
      if (range_up == 4){index_time_up <- 3}
      if (range_up == 6){index_time_up <- 4}
      if (range_up == 7){index_time_up <- 5}
      if (range_up == 9){index_time_up <- 6}
      if (range_up == 10){index_time_up <- 7}
      if (range_up == 13){index_time_up <- 8}
      if (range_up == 18){index_time_up <- 9}
      
      out <- (aux[number_dose-1, index_time_low] - aux[number_dose-1, index_time_up])/6
      return(out)
    }
    
    N <- 100
    gaussian <- function(x, mu, sd){
      out <- exp(- (x-mu)^2/(2*sd^2))/(sd*sqrt(2*pi))
      return(out)
    }
    
    gaussian_2_doses_6 <- function(x){
      out <- gaussian(x, mu=6.976176/100, sd=standard_deviation_sympt(2, 10, 13)/100)*0.07579852
      out
    }
    gaussian_3_doses_6 <- function(x){
      mu <- (0.0000001/100 + 3.741345/100 + 0.1040765/100)/3 
      out <- exp(-x/mu)*1/mu*(0.48689658 + 0.07026471 + 0.03184674)
      out
    }
    gaussian_2_doses_now <- function(x){
      out <- gaussian(x, mu=14.87129/100, sd=standard_deviation_sympt(2, 4, 10)/100)*0.01198989
      out
    }
    gaussian_3_doses_now <- function(x){
      out <- gaussian(x, mu=12.8673/100, sd=standard_deviation_sympt(3, 4, 7)/100)*0.01279443
      out
    }
    gaussian_3_doses_senior <- function(x){
      out <- gaussian(x, mu=52.74899/100, sd=standard_deviation_sympt(3, 1/4, 4)/100)*0.26335747
      out
    }
    gaussian_4_doses <- function(x){
      out <- gaussian(x, mu=12.8673/100, sd=standard_deviation_sympt(3, 4, 7)/100)*0.04705167
      out
    }
    sum_gaussians <- function(x){
      out <- gaussian_2_doses_6(x) + gaussian_2_doses_now(x) + gaussian_3_doses_now(x) + gaussian_3_doses_6(x) + gaussian_3_doses_senior(x) + gaussian_4_doses(x)
      return(out)
    }
    
    out <- data.table::data.table(x = seq(0, 1, 0.0005))
    out[, `:=`(
      sum_gaussians = sum_gaussians(x)
    ), ]
    out %<>% tidyr::gather("incub", "value", -x)
    
    dist <- c()
    for (i in 1:(N+1)){
      x <- (i-1)/N
      dist[[i]] <- sum_gaussians(x)
    }
    max_sum_gaussians <- max(as.numeric(dist))
    mean_sum_gaussians <- 0
    for (i in 1:(N+1)){
      mean_sum_gaussians <- mean_sum_gaussians + (i-1)/N*dist[[i]]*1/N
    }
    mean_sum_gaussians*100
    
    out <- c()
    for (i in 1:n){
      end <- 0
      while(end == 0){
        X <- runif(1, min=0, max=1)
        Y <- runif(1, min=0, max=max_sum_gaussians)
        if (sum_gaussians(X) >= Y){
          out[[i]] <- 0.1*X/(1-0.9*X) + prop.asym*(1 - 0.1*X/(1-0.9*X))
          end <- 1
        }
        end
      }
    }

    return(list(unlist(out), mean(unlist(out)))) 
  }
}
