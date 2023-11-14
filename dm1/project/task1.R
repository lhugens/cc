library(tidyverse)

setwd("~/cc/dm1/project")

# lets start by "district", has the larger number of attributes
district <- read_csv2("bank/district.csv")

summary(district)

## REMOVE IRRELEVANT FEATURES ##
# remove no. of municipalities 
district <- district %>% select(-c(`no. of municipalities with inhabitants < 499`, `no. of municipalities with inhabitants 500-1999`, `no. of municipalities with inhabitants 2000-9999`, `no. of municipalities with inhabitants >10000`))
# remove no. of cities
district <- district %>% select(-c(`no. of cities`))
# rename several attribute
district <- district %>% rename(no_inhabitants = 'no. of inhabitants')
district <- district %>% rename(ratio_urban_inhabitants=`ratio of urban inhabitants`)
district <- district %>% rename(average_salary = `average salary`)
district <- district %>% rename(no_crimes_95 = `no. of commited crimes '95`)
district <- district %>% rename(no_crimes_96 = `no. of commited crimes '96`)
district <- district %>% rename(unemploy_rate_95=`unemploymant rate '95`)
district <- district %>% rename(unemploy_rate_96=`unemploymant rate '96`)
district <- district %>% rename(no_enterpre_per_1000=`no. of enterpreneurs per 1000 inhabitants`)

## TYPE CONVERT ##
# from "summary" output, we now some attributes should have numeric type, we convert them
district <- district %>% mutate(no_crimes_95 = as.numeric(no_crimes_95))
district <- district %>% mutate(unemploy_rate_95 = as.numeric(unemploy_rate_95))
district <- district %>% mutate(unemploy_rate_96 = as.numeric(unemploy_rate_96))

## DEALT WITH MISSING VALUES ##
# are there any missing values "NA"
district %>% filter(is.na(no_crimes_95))
# missing value in entry 69, Jesenik, of region North Moravia
district %>% filter(is.na(no_crimes_96))
# none
district %>% filter(is.na(unemploy_rate_95))
# also entry 69
# to deal with missing values for no_crimes_95, we want to imputate by the mean for that region,
# but we need to normalize by no_inhabitants first, to make sense
district <- district %>% mutate(no_crimes_95_per_1000 = no_crimes_95 * 1000 / no_inhabitants)
district <- district %>% mutate(no_crimes_96_per_1000 = no_crimes_96 * 1000 / no_inhabitants)
# no_crimes_95_per_1000 discover mean per region
mean_crimes_95_per_1000_per_region <- district %>%
    group_by(region) %>%
    summarize(no_crimes_95_per_1000.mean = mean(no_crimes_95_per_1000, na.rm = TRUE)) %>%
    arrange(no_crimes_95_per_1000.mean)
# value to imputate is 41:
# define a function, we will need it for future imputations
imputate_mean_region <- function(data, attribute_name) {
  result <- data %>%
    group_by(region) %>%
    mutate({{ attribute_name }} := ifelse(is.na({{ attribute_name }}), mean({{ attribute_name }}, na.rm = TRUE), {{ attribute_name }}))
  
  return(result)
}
district <- imput_mean_region(district, no_crimes_95_per_1000)
# district <- district %>%
#   group_by(region) %>%
#   mutate(no_crimes_95_per_1000 = ifelse(is.na(no_crimes_95_per_1000), mean(no_crimes_95_per_1000, na.rm = TRUE), no_crimes_95_per_1000))
# one, entry 69 again. lets imputate with average for that region again
district <- district %>% select(-c(no_crimes_95))
district <- district %>% select(-c(no_crimes_96))

# now for unemployment rate
# imputate with mean per region
district <- district %>%
     group_by(region) %>%
     mutate(unemploy_rate_95 = ifelse(is.na(unemploy_rate_95), mean(unemploy_rate_95, na.rm = TRUE), unemploy_rate_95))
# check for 96
district %>% filter(is.na(unemploy_rate_96))
# none

## CREATE NEW ATTRIBUTES ##
# deal with 95 and 96 by keeping the ration of value_96/value_95
district <- district %>% mutate(no_crimes_dif_per_1000 = no_crimes_96_per_1000 - no_crimes_95_per_1000)
district <- district %>% mutate(unemploy_rate_dif = unemploy_rate_96 - unemploy_rate_95)
# and remove the 95 and 96 attributes
district <- district %>% select(-c(unemploy_rate_95))
district <- district %>% select(-c(unemploy_rate_96))

## VISUALIZATION ##
ggplot(district, aes(x = region, y = no_crimes_dif_per_1000)) + geom_boxplot() + ggtitle("No. Commited Crimes Difference Per 1000 inhabitants")
ggplot(district, aes(x = region, y = unemploy_rate_dif)) + geom_boxplot() + ggtitle("Unemployment Rate Difference")
# North Moravia has a decrease in crimes an in unemployment rate, 

## DATA PER REGION ##
# get data per region
district_mean_by_region <- district %>%
      group_by(region) %>%
      summarise_all(mean)
# remove irrelevant attributes: code and name
district_mean_by_region <- district_mean_by_region %>% select(-c(code))
district_mean_by_region <- district_mean_by_region %>% select(-c(name))

## NEXT STEP ##
# client database
# birth_number for women in in format YYMM+50DD, do:
# generate a new attribute, gender, based on the birth_number formatt, and decrease 50 when the MM is >50, to obtain tue birth date 
# generate a new attribute, age, based on the exact birth date




