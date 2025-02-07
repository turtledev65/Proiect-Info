class Popup {
  constructor() {
    const container = document.createElement("div");
    container.classList.add("popup");
    document.body.appendChild(container);

    const title = document.createElement("p");
    title.innerText = "This is a title";
    title.classList.add("popup-title");
    container.appendChild(title);

    const { table, dataCells } = createTable(["Regiune Istorica", "Populatie"]);
    table.classList.add("popup-table")
    container.appendChild(table);

    /** @private
     *  @type {HTMLDivElement}
     */
    this.rootElem = container;
    /** @private
     *  @type {HTMLParagraphElement}
     */
    this.titleElem = title;
    /** @private
     * @type {HTMLTableCellElement}
     */
    this.regionElem = dataCells[0];
    /** @private
     * @type {HTMLTableCellElement}
     */
    this.populationElem = dataCells[1];

    this.setVisible(false);
    this.setPos(0, 0);
  }

  /** @param {number} x
   *  @param {number} y
   *  @param {Area} data
   */
  show(data, x, y) {
    this.setData(data);
    this.setPos(x, y);
    this.setVisible(true);
  }

  hide() {
    this.setVisible(false);
  }

  /**
   * @param {Area} data
   */
  setData(data) {
    this.titleElem.innerText = data.name;
    this.populationElem.innerText = data.totalPopulation;
    this.regionElem.innerText = data.historicalRegion;
  }

  /**
   * @private
   * @param {boolean} val
   */
  setVisible(val) {
    this.rootElem.style.display = val ? "block" : "none";
    this.rootElem.style.opacity = val ? "90%" : "0";
  }

  /**
   * @private
   * @param {number} x
   * @param {number} y
   */
  setPos(x, y) {
    const offset = 10;

    this.rootElem.style.left = `${offset + x}px`;
    this.rootElem.style.top = `${offset + y}px`;
  }
}

/** @type { SVGSVGElement } */
const mapSvg = querySelectorOrThrow("svg.map");
/** @type NodeListOf<SVGPathElement> */
const areas = querySelectorAllOrThrow(".region");
/** @type { HTMLParagraphElement | null } */
const statisticsTitle = querySelectorOrThrow(".statistics-title");

const popup = new Popup();

/** @type { SVGPathElement | null} */
let selectedArea = null;

for (const area of areas) {
  area.addEventListener("click", e => {
    selectArea(area);
  })
  area.addEventListener("mouseover", e => {
    popup.show(getAreaData(area), e.pageX, e.pageY);
  });
  area.addEventListener("mouseleave", () => {
    popup.hide();
  });
}

// Utility functions
/**
 * @param {SVGPathElement} area
 */
function selectArea(area) {
  if (selectedArea == area) {
    return;
  } else if (selectedArea != null) {
    selectedArea.classList.remove("region-selected");
  }

  selectedArea = area;
  area.classList.add("region-selected");

  const title = getAttribOrThrow(area, "title");
  statisticsTitle.innerHTML = `Statistici ${title}`;
}

/**
 * @param {Element} elem
 */
function getAreaData(elem) {
  const id = getAttribOrThrow(elem, "id");
  if (id in AREAS) {
    return AREAS[id];
  }
}

/**
 * @param {string[]} rowNames 
 */
function createTable(rowNames) {
  const table = document.createElement("table");
  const body = document.createElement("tbody");
  table.appendChild(body);

  /** @type {HTMLTableCellElement[]} */
  const dataCells = [];

  for (const name of rowNames) {
    const row = document.createElement("tr");

    const nameCell = document.createElement("td");
    nameCell.innerText = name;
    row.appendChild(nameCell);

    const dataCell = document.createElement("td");
    dataCell.innerText = "...";
    row.appendChild(dataCell);
    dataCells.push(dataCell);

    body.appendChild(row);
  }

  return { table, dataCells };
}

/**
 * @param {string} selector
 * @throws if no element was found
 * @returns {Element}
 */
function querySelectorOrThrow(selector) {
  const elem = document.querySelector(selector);
  if (!elem) {
    throw new Error(`No element was found for ${selector}`);
  }

  return elem;
}

/**
 * @param {string} selector
 * @throws if no element was found
 * @returns {NodeListOf.<Element>}
 */
function querySelectorAllOrThrow(selector) {
  const elems = document.querySelectorAll(selector);
  if (elems.length == 0) {
    throw new Error(`No elements were found for ${selector}`);
  }

  return elems;
}

/**
 * @param {Element} element
 * @param {string} name
 * @returns string
 * @throws if the attribute was not found
 */
function getAttribOrThrow(element, name) {
  const attrib = element.getAttribute(name);
  if (!attrib) {
    throw new Error(`Attribute ${name} not found in ${element.tagName}`);
  }

  return attrib;
}