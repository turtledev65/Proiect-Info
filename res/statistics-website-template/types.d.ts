type Ethnicity = {
  type: number;
  total: number;
};

type Area = {
  name: string;
  totalPopulation: number;
  ethnicities: Ethnicity[];
};
