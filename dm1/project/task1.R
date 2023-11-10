library(tidyverse)

setwd("~/cc/dm1/project")

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
# are there any missing values "NA"
district %>% filter(is.na(no_crimes_95))
# missing value if Jesenik, of region North Moravia
# discover the mean no_crimes_95 per region
mean_crimes_per_region_95 <- 
  district %>%
  group_by(region) %>%
  summarize(no_crimes_95.mean = mean(no_crimes_95, na.rm = TRUE)) %>%
  arrange(no_crimes_95.mean)
# North Moravian has mean 6779.3
# replace the missing value no_crimes_95 with the average of the corresponding region
district <- district %>%
  group_by(region) %>%
  mutate(no_crimes_95 = ifelse(is.na(no_crimes_95), mean(no_crimes_95, na.rm = TRUE), no_crimes_95))
# missing value is correctly amputated

